//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "kunde.h"
#include "ticket_m.h"

Define_Module(Kunde);

double probabilityReopen;
int anzahlFeatures;
double mu = 0.5;

void Kunde::initialize() {
    cMessage *firstmsg = new cMessage("Time");
    firstmsg->setKind(5);
    scheduleAt(simTime() + 1, firstmsg);

    probabilityReopen = par("probReopen").doubleValue();
    anzahlFeatures = par("anzahlFeatures").longValue() - 2;

}

void Kunde::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        handleSelfMessage(msg);
    } else {
        checkResolvedTicket(msg);
    }
}

void Kunde::handleSelfMessage(cMessage *msg) {
    if (msg->getKind() == 5) {
        int countNewTickets = poisson(mu);
        ev<< simTime() << "  " << countNewTickets << " new  --- ";
        for (int i = 0; i < countNewTickets; i++) {
            Ticket *newmsg = new Ticket("Feature " + (100 - anzahlFeatures));
            newmsg->setName("Feature ");
            newmsg->addPar("creationTime");
            newmsg->par("creationTime").setDoubleValue((simTime() + 1).dbl());
            scheduleAt(simTime() + 1, newmsg);
            anzahlFeatures -= 1;
        }
        cMessage *timemsg = new cMessage("Time");
        timemsg->setKind(5);
        scheduleAt(simTime() + 1, timemsg);
    } else {
        send(msg, "out");
    }
}

void Kunde::checkResolvedTicket(cMessage *msg) {
    //feature wurde bearbeitet
    //soll Feature nochmal ueberarbeitet werden?
    double randNumber = dblrand();
    if (randNumber < probabilityReopen) {
        send(msg, "out");
    } else {
        long totaltime = simTime().dbl()
                - msg->par("creationTime").doubleValue();
        ev<< totaltime << " ";
        delete (msg);
        recordScalar("totaltime", totaltime);
    }
}

