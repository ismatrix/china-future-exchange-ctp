#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import os, sys, Ice

slice_dir = Ice.getSliceDir()
if not slice_dir:
    print(sys.argv[0] + ': Slice directory not found.')
    sys.exit(1)

Ice.loadSlice("'-I" + slice_dir + "' -I./  MdPastSession.ice")
import MdPast

class MdSessionCallBackI(MdPast.MdSessionCallBack):
    def onTick(self, TradingDay, InstrumentID, ticker, current=None):
        print(ticker)
        #return 0
    def onBar(self, TradingDay, InstrumentID, bar, period, current=None):
        print(TradingDay, InstrumentID, bar.Timestamp, period)

class Client(Ice.Application):
    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        server = MdPast.MdSessionPrx.checkedCast(self.communicator().propertyToProxy('MDPAST.Proxy'))
        if not server:
            print(self.appName() + ": invalid proxy")
            return 1
        
        adapter = self.communicator().createObjectAdapter("")
        ident = Ice.Identity()
        ident.name = Ice.generateUUID()
        ident.category = ""
        
        adapter.add(MdSessionCallBackI(), ident)
        adapter.activate()
        
        server.ice_getConnection().setAdapter(adapter)
        
        server.queryData(ident,  "M3", "IH1703", "20170116", "20170116"); 

        self.communicator().waitForShutdown()

        return 0

app = Client()
sys.exit(app.main(sys.argv, "config.client"))
