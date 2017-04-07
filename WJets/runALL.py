#!/usr/bin/env python
import os, sys, string
import re, time

#--------------------------------------------------------------------------------
def loopHasNotStarted(logName):

    command = "cat " + logName + " > tmp.log"
    os.system(command)

    catFile = open("tmp.log","r")
    for line in catFile:
        if "Processing :" in line:
            os.system("rm tmp.log")
            return False


    os.system("rm tmp.log")
    return True

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
def dispacher(lepton = "DE", PT = 30, doSyst = 0):

    for i in range(0, 5):
        # Create the .cc file based on runDYJets.cc
        tmpFile = "tmp_" + str(lepton) + "_" + str(PT) + "_" + str(i)
        tmpCCFile = tmpFile + ".cc"
        tmpLogFile = tmpFile + ".log"

        cmdSetJob = "sed "
        cmdSetJob += " -e \"s|bool doSyst.*|bool doSyst = " + str(doSyst) + ";|g\""
        cmdSetJob += " -e \"s|int doWhat.*|int doWhat = " + str(i) + ";|g\""
        cmdSetJob += " -e \"s|string lep.*|string lep = \\\"" + str(lepton) + "\\\";|g\""
        cmdSetJob += " -e \"s|int jetPtMin.*|int jetPtMin = " + str(PT) + ";|g\""
        cmdSetJob += " runDYJets.cc > " + tmpCCFile

        print cmdSetJob
        os.system(cmdSetJob)

        cmdExecJob = "nohup root -b -q " + tmpCCFile + "++ &> " + tmpLogFile + " &"
        print cmdExecJob
        os.system(cmdExecJob)

        # Wait one second to be sure the log file has been created
        time.sleep(1)
        # Wait for the loop to start before going to next job
        while loopHasNotStarted(tmpLogFile):
            time.sleep(2)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
if __name__ == "__main__":
    dispacher("DE", 30)
    #dispacher("DE", 20)

#--------------------------------------------------------------------------------
