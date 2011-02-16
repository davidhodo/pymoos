#!/opt/local/python

import ctypes
import socket 
import sys
from time import *
from threading import Thread, Lock, Event;

sys.path.append('../lib/')

from XPCTcpSocket import *
from CMOOSMsg  import *
from CMOOSCommObject import *
from CMOOSCommPkt import *

class MOOSCommClient( Thread ):

    def __init__(self):
        super( Thread, self).__init__()
        Thread.__init__(self)

        #Define the wire protocol
        self.protocol = u"ELKS CAN'T DANCE 2/8/10";

        self.bConnected	= False;
        self.m_bQuit 	= False;
        self.host	= 'localhost'
        self.port 	= 9000
        self.m_sMyName = 'pythonApp'

        self.m_Outbox = []	
        self.m_Inbox = []	

        self.sock = XPCTcpSocket(9000);
        self.comms = MOOSCommObject();

        self.m_Outbox_Lock = Lock();
        self.m_Inbox_Lock = Lock();

    def Close( self ):
        '''Notify thread about closing connection'''
        self.m_bQuit = True
        self.join()

    def Notify( self, variable, value, time=-1):
        '''Add a message to the outbox'''

        Msg = MOOSMsg( 'N', variable, value, time )

        if self.__Post( Msg ):
            return True
        else:
            return False

    def Register( self, variable, interval ):
        '''Register with the DB for some variable'''
        if not variable:
            return False

        MsgR = MOOSMsg( 'R', variable, interval, -1 )

        if self.__Post( MsgR ):
            return True
        else:
            return False

    def UnRegister( self, variable ):
        '''De-Register with the DB for some variable'''
        print 'UnRegister'

    def IsConnected( self ):
        return self.bConnected;

    def SetCommsTick( self, nCommTick ):
        if nCommTick > MAX_TICK:
            self.mFundamentalFrequency = MAX_TICK
        else:
            self.mFundamentalFrequency = nCommTick 

    def Fetch( self ):
        '''Called by client to retrieve non-null MOOS Messages'''
        #self.m_Inbox_Lock.acquire()
        data = self.m_Inbox	
        #self.m_Inbox_Lock.release()
        self.m_Inbox = []
        return data;

    def Run( self, host, port, myName, fundamentalFreq=5):
        '''Entry point for the MOOS exchange'''
        self.host 	= host 
        self.port 	= port
        self.m_sMyName 	= myName 
        self.mFundamentalFrequency = fundamentalFreq
        self.daemon 	= True
        self.start()

        return True;

    def SetOnConnectCallBack( self, fn ):
        self.onConnectCallBack = fn
        return True;

    def SetOnDisconnectCallBack( self, fn ):
        self.onDisconnectCallBack = fn
        return True;

    def SetOnMailCallBack( self, fn ):
        self.onMailCallBack = fn
        return True;

    def PeekMail( self, MOOSMSG_LIST, Key, erase=False, findYoungest=False ):

        for message in self.m_Inbox:
            if message.m_sKey == Key:
                return message

        return None

    def ServerRequest( self, sWhat, timout=-1, bClear=False ):
        print 'Not yet implemented'	

    def Peek( self, MOOSMSG_LIST, Key, erase=True  ):
        print 'Not yet implemented'

    def GetLocalIPAddress( self ):
        ip = socket.gethostbyaddr( socket.gethostname() )
        return ip[2]

    def run( self ):
        '''Threading entry point'''
        self.__ClientLoop()
        return True;

    def __ClientLoop( self ):

        while not self.m_bQuit:

            if ( self.__ConnectToServer() ):
                self.bConnected = True

                while not self.m_bQuit:
                    sleep( 1/self.mFundamentalFrequency )

                    if not self.__DoClientWork():
                        break

            else:
                print 'Unable to connect to %s'	 % self.host	
                sleep( 1 )				

    def __ConnectToServer( self ):
        if ( self.IsConnected() ):
            print 'Already connected'
            return True;
        try:
            self.sock.vConnect( self.host )	

            if ( self.__HandShake() ):
                return True
        except:
            return False;

    def __DoClientWork( self ):
        '''Main I/O loop'''

        PktTx = CMOOSCommPkt();
        PktRx = CMOOSCommPkt();

        if not self.m_Outbox:
            '''Send a message to tick things over'''
            msg = MOOSMsg()
            msg.m_sSrc = self.m_sMyName;
            self.m_Outbox.append( msg )	

        try:
            self.m_Outbox_Lock.acquire()
            PktTx.Serialize( self.m_Outbox, True, True, None )
            self.m_Outbox_Lock.release()
        except:
            return False
        self.m_Outbox = []


        #Get the local time
        #dfLocalPktTxTime = MOOSLocalTime()

        if not self.comms.SendPkt( self.sock, PktTx ):
            print 'Send error'
            return False

        if not self.comms.ReadPkt( self.sock, PktRx, -1 ):
            print 'Receive error'
            return False

        #dfLocalPktRxTime = MOOSLocalTime();
        #PktRx.Serialize( m_Inbox, false, true, dfServerPktTxTime )
        self.m_Inbox_Lock.acquire()
        PktRx.Serialize( self.m_Inbox, False, True, None )
        
        if self.onMailCallBack: 
            try:
                self.onMailCallBack()
            except:
                print 'Unable to evaluate user-specified on-mail callback'

        self.m_Inbox_Lock.release()

        #Clear the outbox
        self.m_Outbox = []

        return True

    def __HandShake( self ):

        #Send the wire protocol
        #c = ctypes.c_char_p( u"ELKS CAN'T DANCE 2/8/10" );
        c = ctypes.c_char_p( self.protocol )
        v = ctypes.cast( c, ctypes.c_void_p )
      
        self.sock.SendMessage( v.value, 32);

        Msg = MOOSMsg( 'D', '', self.m_sMyName , 1.0 )
        self.comms.SendMsg( self.sock, Msg  )

        WelcomeMsg = MOOSMsg();
        self.comms.ReadMsg( self.sock, WelcomeMsg )

        if ( WelcomeMsg.IsType( 'K' ) ):
            print 'Client was poisioned'
            return False

        self.skew = WelcomeMsg.m_dfVal

        #Invoke onconnect callback
        try:
            self.onConnectCallBack();
        except:
            print 'Unable to evaluate user-specified callback'

        return True

    def __Post( self, message ):
        '''Called by client to send a message to the MOOS DB'''

        try:
            message.m_sSrc = self.m_sMyName;

            self.m_Outbox_Lock.acquire()
            self.m_Outbox.append( message )
            self.m_Outbox_Lock.release()

            return True
        except:
            return False



class MOOSApp( MOOSCommClient ):
    '''Test class for the MOOSCommClient class'''

    def __init__(self):
        MOOSCommClient.__init__(self)

    def DoRegistrations( self ):
        self.Register( 'counter', 0.0 );

    def MailCallback( self ):
        print 'Received mail'

    def GetMail( self ):	
        return self.Fetch();

if __name__ == "__main__":

    m = MOOSApp();
    m.SetOnConnectCallBack( m.DoRegistrations )
    m.SetOnMailCallBack( m.MailCallback )
    
    print '%s' % m.GetLocalIPAddress()

    m.Run( '127.0.0.1', 9000, 'test_me', 100) 

    counter = 0;

    while True:
        sleep(.01)
        messages = m.GetMail()

        m.Notify( "counter", counter );
        counter += 1;

        #for message in messages:
            #print 'Message trace:'
            #print message.Trace()
            #print 'Message key:'
            #print message.GetKey()
            #print 'Message time:'
            #print message.GetTime()
