#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int main(int argc, char* argv[]) {

    uint32_t nNodes = 7;
    double starttime = 1.0;
    double endtime = 10.0;
    uint32_t nlinks = 6;
    uint32_t startnode = 0;
    uint32_t endnode = 3;
    std::vector<Ipv4Address> id = {
        "34.1.1.0",
        "34.1.2.0",
        "34.1.3.0",
        "34.1.4.0",
        "34.1.5.0",
        "34.1.6.0"
    };

    CommandLine cmd;
    cmd.AddValue("nNodes", "Number of nodes", nNodes);
    cmd.AddValue("starttime", "Start time of server and client", starttime);
    cmd.AddValue("endtime", "End time of server and client", endtime);
    cmd.AddValue("nlinks", "Number of links", nlinks);
    cmd.AddValue("startnode", "Server node", startnode);
    cmd.AddValue("endnode", "Client Node", endnode);
    cmd.AddValue("id1", "Net ID of device 1", id[0]);
    cmd.AddValue("id2", "Net ID of device 2", id[1]);
    cmd.AddValue("id3", "Net ID of device 3", id[2]);
    cmd.AddValue("id4", "Net ID of device 4", id[3]);
    cmd.AddValue("id5", "Net ID of device 5", id[4]);
    cmd.AddValue("id6", "Net ID of device 6", id[5]);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NodeContainer nodes;
    nodes.Create(nNodes); // 7 nodes

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices1, devices2, devices3, devices4, devices5, devices6;
    devices1 = p2p.Install(nodes.Get(0), nodes.Get(1));
    devices2 = p2p.Install(nodes.Get(0), nodes.Get(2));
    devices3 = p2p.Install(nodes.Get(1), nodes.Get(3));
    devices4 = p2p.Install(nodes.Get(1), nodes.Get(4));
    devices5 = p2p.Install(nodes.Get(2), nodes.Get(5));
    devices6 = p2p.Install(nodes.Get(2), nodes.Get(6));

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase(id[0], "255.255.255.0");
    Ipv4InterfaceContainer interfaces1 = address.Assign(devices1);

    address.SetBase(id[1], "255.255.255.0");
    Ipv4InterfaceContainer interfaces2 = address.Assign(devices2);

    address.SetBase(id[2], "255.255.255.0");
    Ipv4InterfaceContainer interfaces3 = address.Assign(devices3);

    address.SetBase(id[3], "255.255.255.0");
    Ipv4InterfaceContainer interfaces4 = address.Assign(devices4);

    address.SetBase(id[4], "255.255.255.0");
    Ipv4InterfaceContainer interfaces5 = address.Assign(devices5);

    address.SetBase(id[5], "255.255.255.0");
    Ipv4InterfaceContainer interfaces6 = address.Assign(devices6);

    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(6));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(interfaces6.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(3));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    p2p.EnablePcapAll("21DCE034");

    AnimationInterface anim("21DCE034.xml");

    anim.SetConstantPosition(nodes.Get(0), 20.0, 5.0);
    anim.SetConstantPosition(nodes.Get(1), 20.0, 10.0);
    anim.SetConstantPosition(nodes.Get(2), 30.0, 10.0);
    anim.SetConstantPosition(nodes.Get(3), 25.0, 20.0);
    anim.SetConstantPosition(nodes.Get(4), 15.0, 20.0);
    anim.SetConstantPosition(nodes.Get(5), 35.0, 20.0);
    anim.SetConstantPosition(nodes.Get(6), 45.0, 20.0);

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
