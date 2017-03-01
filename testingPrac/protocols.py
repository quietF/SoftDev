# Copyright (c) 2014 The University of Edinburgh.

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


from twisted.internet.protocol import Protocol, ProcessProtocol
from twisted.internet import reactor, threads
#import taskp as task # Use my version
from twisted.internet import taskp as task # Use my version
from twisted.internet.endpoints import connectProtocol
import json
import config
import time
import re
import random
import os


class Trigger(Protocol):
    """
    This protocol triggers a service
    """

    def __init__(self, src, dst, sv, duration, traffic, bidirectional, ditgoptarg):
        self.src = src
        self.dst = dst
        self.key = config.key
        self.svc = sv
        self.duration = duration
        self.data = []
        self.tstart = 0
        self.tstop = 0
        self.traffic = traffic
        self.optarg = ditgoptarg
        if bidirectional == "random":
            r = random.randint(0,1)
            if r==1:
                self.bidirectional = True
            else:
                self.bidirectional = False
                if self.traffic!=False:
                    self.traffic = self.traffic.split("/")[0]
        else:
            self.bidirectional = bidirectional

        self.cc = self.logPrefix() + src + dst
        config.current_connections.append(self.cc)

        if config.debug:
            print self.traffic, self.bidirectional, src, dst, sv, duration, ditgoptarg

    def dataReceived(self, data):
        self.data.append(data)

    def connectionLost(self, reason):
        self.tstop = time.time()
        if self.data:
            incoming_data = json.loads(''.join(self.data))
            if incoming_data['code'] == 0 and incoming_data['type'] == 1:
                print "Started session %s" % incoming_data['session']
                task.deferLater(reactor, self.duration, drop_session, incoming_data['session'])
                config.current_connections.append("Drop" + str(incoming_data['session']))
                if self.traffic:
                    if self.bidirectional==False:
                        prot2 = TriggerDITG()
                        psrc = self.traffic.split("/")[0]
                        pdst = self.dst
                        pargs = ["ITGManager", psrc, "-a", pdst, "-t", str(self.duration * 1000), self.optarg]
                        penv = {'LD_LIBRARY_PATH': os.environ['LD_LIBRARY_PATH']}
                        # if config.debug:
                        #     print pargs
                        config.logger.info("DATA, VPS ID %s" % (incoming_data['session']))
                        config.reactor.spawnProcess(prot2, "./ITGManager", pargs, penv)
                    if self.bidirectional==True:
                        prot2 = TriggerDITG()
                        psrc = self.traffic.split("/")[0]
                        pdst = self.dst
                        pargs = ["ITGManager", psrc, "-a", pdst, "-t", str(self.duration * 1000), self.optarg]
                        penv = {'LD_LIBRARY_PATH': os.environ['LD_LIBRARY_PATH']}
                        config.logger.info("DATABIDIR, VPS ID %s" % (incoming_data['session']))
                        config.reactor.spawnProcess(prot2, "./ITGManager", pargs, penv)
                        prot3 = TriggerDITG()
                        psrc = self.traffic.split("/")[1]
                        pdst = self.src # This is it going the other way round!
                        pargs = ["ITGManager", psrc, "-a", pdst, "-t", str(self.duration * 1000), self.optarg]
                        penv = {'LD_LIBRARY_PATH': os.environ['LD_LIBRARY_PATH']}
                        config.logger.info("DATABIDIR, VPS ID %s" % (incoming_data['session']))
                        config.reactor.spawnProcess(prot3, "./ITGManager", pargs, penv)
                        

            else:
                if config.debug:
                    print incoming_data
                print "Velox Error %s" % decode_error_code(incoming_data['code'], self.src, self.dst)

            config.logger.info("VPS time %d %f, VPS ID %s" % (1, self.tstop - self.tstart, incoming_data['session']))
        else:
            config.logger.info(" TriggerError NoData %s", reason)

        config.current_connections.remove(self.cc)

    def connectionMade(self):
        self.tstart = time.time()
        dict_data = {"key": self.key, "type": 1, "source": self.src, "destination": self.dst, "service": self.svc,
                     "bidirectional": str(self.bidirectional)}
        if config.debug:
            print dict_data
        self.transport.write(json.dumps(dict_data))
        self.transport.loseWriteConnection()


class TriggerDITG(ProcessProtocol):
    """
    Use this to communicate with ITGManager running locally
    """

    def __init__(self):
        if config.debug:
            print "In TDITG"
        self.data = ""

    def outReceived(self, data):
        if config.debug:
            print data
        if data.split()[3] == "2":
            self.transport.signalProcess('KILL')

    def errReceived(self, data):
        if config.debug:
            print data

class Drop(Protocol):
    """
    This protocol drops a service
    """

    def __init__(self, sid):
        self.key = config.key
        self.sess_id = sid
        self.data = []
        self.tstart = 0
        self.tstop = 0
        self.cc = self.logPrefix() + self.sess_id
        config.current_connections.append(self.cc)


    def dataReceived(self, data):
        self.data.append(data)

    def connectionLost(self, reason):
        self.tstop = time.time()
        if self.data:
            incoming_data = json.loads(''.join(self.data))
            if incoming_data['code'] == 0 and incoming_data['type'] == 2:
                print "Dropped session %s" % self.sess_id
            else:
                print incoming_data
                print "Velox Error %s" % decode_error_code(incoming_data['code'], 0, 0)
            config.logger.info("VPS time %d %f" % (2, self.tstop - self.tstart))
        else:
            config.logger.info(" DropError NoData %s", reason)


        config.current_connections.remove(self.cc)
        config.current_connections.remove("Drop" +  str(self.sess_id))


    def connectionMade(self):
        self.tstart = time.time()
        dict_data = {"key": self.key, "type": 2, "session": self.sess_id}
        self.transport.write(json.dumps(dict_data))
        self.transport.loseWriteConnection()


class ListAvailable(Protocol):
    """ This protocol lists all the currently active sessions"""

    def __init__(self):
        self.key = config.key
        self.data = []
        self.tstart = 0
        self.tstop = 0

    def dataReceived(self, data):
        self.data.append(data)

    def connectionLost(self, reason):
        self.tstop = time.time()
        if self.data:
            incoming_data = json.loads(''.join(self.data))
            if incoming_data['code'] == 0 and incoming_data['type'] == 4:
                inc_sess = incoming_data['services'] # For simplicity in referencing
                if len(inc_sess) > 0:
                    print "Available services\nID\tName\t\tBandwidth\tType"
                    for i in range(0, len(inc_sess)):
                        print "%d\t%s\t\t%d\t\t%s" % (
                            inc_sess[i]['id'], inc_sess[i]['name'], inc_sess[i]['bandwidth'], inc_sess[i]['type'])
            else:
                print "Velox Error %s" % decode_error_code(incoming_data['code'], 0, 0)
            config.logger.info("VPS time %d %f" % (4, self.tstop - self.tstart))
        else:
            config.logger.info(" ListAvail NoData %s" % (reason))
            print reason


    def connectionMade(self):
        self.tstart = time.time()
        dict_data = {"key": self.key, "type": 4}
        self.transport.write(json.dumps(dict_data))
        self.transport.loseWriteConnection()


class ListRunning(Protocol):
    """ This protocol lists all the currently active sessions"""

    def __init__(self):
        self.key = config.key
        self.data = []
        self.tstart = 0
        self.tstop = 0

    def dataReceived(self, data):
        self.data.append(data)

    def connectionLost(self, reason):
        self.tstop = time.time()
        if self.data:
            incoming_data = json.loads(''.join(self.data))
            if incoming_data['code'] == 0 and incoming_data['type'] == 5:
                inc_sess = incoming_data['sessions']
                if len(inc_sess) > 0:
                    print "Running sessions\nID\tSession ID"
                    for i in range(0, len(inc_sess)):
                        print "%d\t%s" % (i, inc_sess[i]['id'])
                else:
                    print "No sessions running"
            else:
                print "Velox Error %s" % decode_error_code(incoming_data['code'], 0, 0)
            config.logger.info("VPS time %d %f" % (5, self.tstop - self.tstart))
        else:
            config.logger.info(" ListRunning NoData %s" % (reason))

    def connectionMade(self):
        self.tstart = time.time()
        dict_data = {"key": self.key, "type": 5}
        self.transport.write(json.dumps(dict_data))
        self.transport.loseWriteConnection()


class DropAll(Protocol):
    """ This protocol drops all the currently active sessions"""

    def __init__(self):
        self.key = config.key
        self.data = []
        self.tstart = 0
        self.tstop = 0
        # config.enditall.cancel()

    def dataReceived(self, data):
        self.data.append(data)

    def connectionLost(self, reason):
        self.tstop = time.time()
        if self.data:
            incoming_data = json.loads(''.join(self.data))
            if incoming_data['code'] == 0 and incoming_data['type'] == 5:
                inc_sess = incoming_data['sessions']
                if len(inc_sess) > 0:
                    print "Running sessions\nID\tSession ID"
                    for i in range(0, len(inc_sess)):
                        print "%d\t%s" % (i, inc_sess[i]['id'])
                        config.current_connections.append("Drop" + str(inc_sess[i]['id']))
                        threads.deferToThread(drop_session, inc_sess[i]['id'])
                else:
                    print "No sessions running"
            else:
                print "Velox Error %s" % decode_error_code(incoming_data['code'], 0, 0)
            config.logger.info("VPS time %d %f" % (9, self.tstop - self.tstart))
        else:
            config.logger.info(" DropAll NoData %s" % (reason))
            print reason

    def connectionMade(self):
        self.tstart = time.time()
        dict_data = {"key": self.key, "type": 5}
        self.transport.write(json.dumps(dict_data))
        self.transport.loseWriteConnection()


# Scheduling functions for Protocol derivatives

def drop_all(sid):
    config.logger.info("Droping all sessions at request of %d", sid)
    prot = DropAll()
    d = connectProtocol(config.point, prot)
    d.addErrback(err1)


def trigger_session(src_range, dst_range, **kwargs):
    """ This function triggers a single session by creating an instance of the Trigger
    class and then calling the connectProtocol method on its deffered.
    It also adds an errBack so we can see if it fails in the logs."""

    # Default arguements
    service_type = config.default_service_type
    bidirectional = config.default_bidirectional
    session_duration = config.default_session_duration
    traffic = config.default_traffic
    ditgoptarg = config.default_ditgoptarg

    # Generate a randomized IP if required
    src, dst = randomize_ip(src_range, dst_range)

    # Deal with any optionally supplied kwargs
    if kwargs.has_key('service_type'):
        service_type = kwargs['service_type']

    if kwargs.has_key('session_duration'):
        session_duration = kwargs['session_duration']
        if isinstance(session_duration, list):
            sd = random.randint(session_duration[0], session_duration[1])
            print "SD = %d" % sd
            session_duration = float(sd)

    if kwargs.has_key('exponential'):
        session_duration = random.expovariate(1.0/session_duration)

    if kwargs.has_key('traffic'):
        traffic = kwargs['traffic']

    if kwargs.has_key('bidirectional'):
        bidirectional = kwargs['bidirectional']

    if kwargs.has_key('ditgoptarg'):
        ditgoptarg = kwargs['ditgoptarg']

    # Create an instance of Trigger & call its connect method
    prot = Trigger(src, dst, service_type, session_duration, traffic, bidirectional, ditgoptarg)
    d = connectProtocol(config.point, prot)
    d.addErrback(err1)


def n1(e):
    print "This is N1"
    


def drop_session(session_id):
    prot = Drop(session_id)
    d = connectProtocol(config.point, prot)
    d.addErrback(err1)


def new_loop_job(src, dst, interval, loop_duration, **kwargs):
    a = trigger_session, src, dst
    l = task.LoopingCall(threads.deferToThread, *a, **kwargs)
    task.deferLater(config.reactor, loop_duration, lambda: l.stop())
    l.start(interval)


def list_running(sid):
    prot = ListRunning()
    d = connectProtocol(config.point, prot)
    d.addErrback(err1)


def list_avail_services(sid):
    prot = ListAvailable()
    d = connectProtocol(config.point, prot)
    d.addErrback(err1)


def err1(e):
    config.logger.info(" ERRBACK1 %s", e)

# Decode Error Codes
def decode_error_code(err_code, s, d):
    """ This function decodes the error codes received from VELOX"""

    config.logger.warn('Failure: %d %s %s', err_code, s, d)

    return {
        0: 'Request completed successfully. No error',
        1: 'Invalid API key',
        2: 'Unknown Request',
        3: 'Invalid arguements',
        4: 'Invalid service',
        5: 'Invalid session',
        6: 'Insufficient bandwidth available',
        7: 'No path between src and dst with that service type',
        8: 'Internal VELOX error',
        9: 'Nothing to modify',
        -1: 'Server comms error',
    }.get(err_code, 'Unknown error code')


def randomize_ip(src, dst):
    """ Detect and randomize IP addresses within a block if necessary. """

    test_src = re.search("/", src)
    test_dst = re.search("/", dst)

    if test_src:
        src_prefix = int(re.split("/", src)[1])
        src_prefix_length = 4 - (src_prefix / 8)
        z = re.split("\.", src)[:4 - src_prefix_length]

        for i in range(0, src_prefix_length):
            if i == src_prefix_length - 1:
                r = str(random.randint(2, (pow(2, 8) - 2)))
            else:
                r = str(random.randint(0, (pow(2, 8) - 1)))
            z.append(r)

        new_src = '.'.join(z[i] for i in range(0, len(z)))
        src = new_src

    if test_dst:
        dst_prefix = int(re.split("/", dst)[1])
        dst_prefix_length = 4 - (dst_prefix / 8)
        z = re.split("\.", dst)[:4 - dst_prefix_length]

        for i in range(0, dst_prefix_length):
            if i == dst_prefix_length - 1:
                r = str(random.randint(1, (pow(2, 8) - 2)))
            else:
                r = str(random.randint(0, (pow(2, 8) - 1)))
            z.append(r)

        new_dst = '.'.join(z[i] for i in range(0, len(z)))
        dst = new_dst

    return (src, dst)
