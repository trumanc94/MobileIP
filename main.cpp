/*
Authors: Truman Chan, Marco Rivas, David Mar
CPE 400
Project 2
Topic: Mobile IP
*/

/*
Abstract: Mobile IP allows mobile devices to keep their same IP address when they move between 
		  networks. The main goal with IP mobility is to maintain a relatively unchanged TCP 
		  connection between a moving mobile device and its corresponding host. 
*/

// Header Files
#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <thread>
#include <chrono>

using namespace std;

// Classes 
/*
The ICMP class is used during the agent discovery portion of mobile IP. Advertisements from
home agents and foreign agents, along with the solicitation message from mobile nodes are
ICMP messages
*/
enum ICMP_t { ADVERTISEMENT, SOLICITATION }; // advertisement is type 9, solicitation is type 10
class ICMP
{
	public:
		// Constructor
		ICMP( ICMP_t t, string i, bool home, bool foreign, bool registration )
			: type(t), IP(i), H(home), F(foreign), R(registration) {}

		// Member Functions
		void insertCOA(string careOfAddress)
		{
			COA.push_front(careOfAddress);
		}

		string getCOA()
		{
			string address = COA.front();
			COA.pop_front();
			return address;
		}

		void printICMP()
		{
			cout << "ICMP: Type(";
			if(type == ADVERTISEMENT) cout << "9/ADVERTISEMENT";
			else cout << "10/SOLICITATION";
			cout << "), IP(" << IP << "), H(" << H << "), F(" << F << "), R(" << R << ")" << endl << endl;
		}

	private:
		// Data members
		ICMP_t type;		// ADVERTISEMENT or SOLICITATION
		string IP;			// IP address
		bool H;				// Home agent bit
		bool F;				// Foreign agent bit
		bool R;				// Registration required bit
		list<string> COA;	// List of available Care-of-Addresses in foreign network
};

/*
The registration message class is used during the registration portion of mobile IP. 
Registration messages are sent inside UDP datagrams between the mobile node, foreign agent,
and home agent.
*/
enum registration_t { REQUEST, REPLY };
class registrationMessage
{
   public:
	    // Constructor
		registrationMessage( registration_t type, string c, string h, string m, int l, int i )
			: registerType(type), COA(c), HAAddress(h), MNAddress(m), lifeTime(l), id(i) {}

		// Member Functions
		registration_t getRegisterType() { return registerType; }
		string getCOA(){ return COA; }
		string getHAAddress(){ return HAAddress; }
		string getMNAddress(){ return MNAddress; }
		int getLifetime(){ return lifeTime; }
		int getID(){ return id; }

		void printRegistration(bool encapsulation)
		{
			if(encapsulation) cout << "[ENCAPSULATED] ";
			cout << "Registration(";
			if(registerType == REQUEST) cout << "Request): ";
			else cout << "Reply): ";			
			if(COA != "") cout << "COA(" << COA << "), ";
			cout << "HA(" << HAAddress << "), MA(" << MNAddress << "), Lifetime(" << lifeTime << "), ID(" << id << ")";			
			cout << endl << endl;
		}

   private:
	   // Data Members
   	   registration_t registerType; // REQUEST or REPLY
	   string COA;					// Care-of-Address of mobile node in foreign network
	   string HAAddress;			// Home agent address
	   string MNAddress;			// Mobile node permanent address
	   int lifeTime;				// Lifetime of requested registration
	   int id;						// 64-bit ID of message (Acts like sequence number to match REQUEST/REPLY)
};

/*
Mobile node in this simulator is any device that is moving across networks. It is the 
source and destination of datagrams sent between the correspondent node.
*/
class mobileNode
{
   public:
      // Constructor
      mobileNode(string internetProtocol, string MACAddress) : IP (internetProtocol), MAC(MACAddress) {}
      
      // Member Functions
      string getIP() { return IP; }
      string getMAC() { return MAC; }
	  void setCOA(string careOfAddress) { COA = careOfAddress; }
	  string getCOA() { return COA; }
      
   private:
      // Members
      string IP;   // This is the permanent IP of the MN's home address 
      string MAC;
	  string COA;  // Care-of-Address/current location of mobile node
};

/*
Correspondent node in this simulator is the entity communicating with the mobile node.
*/
class correspondentNode
{
	public: 
		// Constructor
		correspondentNode(string internetProtocol) {IP = internetProtocol; }

		// Member Functions
		string getIP()  { return IP; }

	private:
		// Members
		string IP;  // IP for the web server, etc.
};

/*
Home agent is the entity in a home network that performs the mobility management functions
for the mobile node, such as forwarding packets to a foreign agent of a foreign network in
which the mobile node is located.
*/
class homeAgent
{
   public:
      // Constructor
      homeAgent(string HA) { HAAddress = HA; }
      
      // Member Functions
      string getHA() { return HAAddress; }

      void addEntry(string home, string coa, int time)
      {
         // Add new binding entry to Mobility Binding Table
         bindingEntry temp(home, coa, time);
         bindingTable.push_front(temp);
      }

      void printEntries()
      {
         // Print Binding Table title
         cout << "-----------------------------------------------------";
         cout << endl;
         cout << "               Mobility Binding Table                ";
         cout << endl;
         cout << "-----------------------------------------------------";
         cout << endl;
         cout << "|  Home Address   | Care-of-Address | Lifetime(sec) |";
         cout << endl;
         cout << "-----------------------------------------------------";
         cout << endl;
                  
         // Iterate through Mobility Binding Table and print each entry
         std::list<bindingEntry>::const_iterator iterator;
         for(iterator = bindingTable.begin(); 
                              iterator != bindingTable.end(); ++iterator)
         {
             cout << "| " << (*iterator).homeAddress;
             printSpaceAndBar((*iterator).homeAddress);
             cout << (*iterator).COA;
             printSpaceAndBar((*iterator).COA);
             printLifeTime((*iterator).lifetime);
             cout << " |" << endl;             
         }
      }
      
   private:
      // Mobility Binding Table entries
      class bindingEntry
      {
         public:
            // Constructor
            bindingEntry(string home, string careOfAddress, int time)
                  :homeAddress(home), COA(careOfAddress), lifetime(time) {}
          
            // Members
            string homeAddress;  // Home address of a mobility node
            string COA;          // Care-of-Address of a mobility node
            int lifetime;        // Lifetime of the entry in seconds
      };

      // Member Functions
      void printSpaceAndBar(string IP)
      {
		for (unsigned i = 0; i < (15 - IP.length()); i++) cout << " ";
		cout << " | ";
      }
       
      void printLifeTime(int val)
      {
		int leftSpace = 6;
		int rightSpace = 6;
		int v = val / 10;
		while (v > 0)
		{
			if (leftSpace == rightSpace)
				leftSpace--;
			else
				rightSpace--;

			v /= 10;
 		}

		for (int i = 0; i < leftSpace; i++)	cout << " ";
		cout << val;
 		for (int i = 0; i < rightSpace; i++) cout << " ";
	 }

      // Data Members
      string HAAddress;                // Home Agent address
      list<bindingEntry> bindingTable; // Mobility Binding Table     
};

/*
Foreign agent is an entity inside a foreign network that helps the mobile node with mobility
management functions, such as receiving datagrams addressed to a mobile node located in its
network.
*/
class foreignAgent
{
   public:
      // Constructor
      foreignAgent(string FA) { FAAddress = FA; }
         
      // Member Functions
      string getFA() { return FAAddress; }         

      void addEntry(string home, string HA, string MAC, int time)
      {
         // Add new binding entry to Mobility Binding Table
         visitorEntry temp(home, HA, MAC, time);
         visitorList.push_front(temp);
      }

      void printEntries()
      {
         // Print Binding Table title
         cout << "-------------------------------------------------------------------------";
         cout << endl;
         cout << "                              Visitor List                               ";
         cout << endl;
         cout << "-------------------------------------------------------------------------";
         cout << endl;
         cout << "|  Home Address   |   Home Agent    |   Media Address   | Lifetime(sec) |";
         cout << endl;
         cout << "|                 |     Address     |                   |               |";
         cout << endl;
         cout << "-------------------------------------------------------------------------";
         cout << endl;
                  
         // Iterate through Mobility Binding Table and print each entry
         std::list<visitorEntry>::const_iterator iterator;
         for(iterator = visitorList.begin(); 
                              iterator != visitorList.end(); ++iterator)
         {
             cout << "| " << (*iterator).homeAddress;
             printSpaceAndBar((*iterator).homeAddress);
             cout << (*iterator).HAAddress;
             printSpaceAndBar((*iterator).HAAddress);
             cout << (*iterator).mediaAddress << " | ";
             printLifeTime((*iterator).lifetime);
             cout << " |" << endl;
         }
      }
               
   private:
      // Visitor List entries
      class visitorEntry
      {
         public:
            // Constructor
            visitorEntry(string home, string HA, string MAC, int time)
                  :homeAddress(home), HAAddress(HA), mediaAddress(MAC),
                                                          lifetime(time){}
                                                          
            // Members
            string homeAddress;  // Home address of a mobility node
            string HAAddress;    // Home agent address
            string mediaAddress; // MAC address
            int lifetime;        // Lifetime of the entry in seconds
      };
      
      // Member Functions
      void printSpaceAndBar(string IP)
      {
		for (unsigned i = 0; i < (15 - IP.length()); i++) cout << " ";
		cout << " | ";
      }
      
      void printLifeTime(int val)
      {
		int leftSpace = 6;
		int rightSpace = 6;
		int v = val / 10;
		while (v > 0)
		{
			if (leftSpace == rightSpace)
			leftSpace--;
			else
			rightSpace--;

			v /= 10;
 		}

		for (int i = 0; i < leftSpace; i++)	cout << " ";
		cout << val;
 		for (int i = 0; i < rightSpace; i++) cout << " ";
	  }
      
      // Data Members
      string FAAddress;               // Foreign Agent address
      list<visitorEntry> visitorList; // Visitor List           
};

/*
The datagram class contains the data sent between mobile node and correspondent node. It
contains the source and destination address, as well as a time-to-live. Datagrams are
encapsulated to keep the correspondent's datagram intact when it needs to be sent to a home
agent or foreign agent. The application receiving the datagram should be unwaware that the 
datagram was forwarded via the home agent. Once an encapsulated datagram is received by the 
foreign agent, it is decapsulated by removing the original correspondent datagram and 
forwarded to the mobile node.
*/
class datagram
{
   public:
      // Constructor
      datagram(string src, string dest, int time, int i)
               :source(src), destination(dest), TTL(time), ID(i){}
               
      // Member Functions
      string getSrc() { return source; }
      string getDest() { return destination; }
      int getTTL() { return TTL; }
      void print() {

          cout << "Src: " << source << " Dest: " << destination << " TTL: " << TTL << " Id: " << ID << endl << endl;
	  }
	
	  void decreaseTTL(){TTL--;}

   private:
      // Members
      string source;    // Source address
      string destination;     // Destination address
      int TTL;  // Lifetime of the registration entry
      int ID;        // Identification number of the datagram      
};

// Function Prototype Declarations
void Sleep(int);
string generateIP();
string generateMAC();
void agentDiscovery(mobileNode&, homeAgent, foreignAgent, char);
void registerMN(mobileNode, homeAgent, foreignAgent);
void indirectRouting(mobileNode, homeAgent, foreignAgent, correspondentNode);
void directRouting(mobileNode, homeAgent, foreignAgent, correspondentNode);
void routeOptimization(mobileNode, correspondentNode);

// Main Simulation
int main()
{
	// Seed time
	srand((unsigned int) time(NULL));

    // Initialize objects
    mobileNode MN(generateIP(), generateMAC());
	homeAgent HA(MN.getIP().replace(MN.getIP().find_last_of("."), 4, "." + to_string(rand() % 254 + 1)));
    foreignAgent FA(generateIP());
    correspondentNode CN(generateIP());
	char selection;
	bool keepRunning = true;

	// Display information
	cout << endl << "----------------------INFORMATION------------------------" << endl << endl;
	cout << "Mobile Node IP: " << MN.getIP() << endl << endl;
	cout << "Home Agent address: " << HA.getHA() << endl;
	HA.printEntries();
	cout << endl;
	cout << "Foreign Agent address: " << FA.getFA() << endl;
	FA.printEntries();
	cout << endl;
	cout << "----------------------INFORMATION------------------------" << endl << endl << endl;

    // Display Main Menu
	do {
		cout << "---------------------------------------------------------" << endl;
		cout << "                        Mobile IP                        " << endl;
	    cout << "---------------------------------------------------------" << endl;
	    cout << "Select if mobile node is in (H)ome or (F)oreign network: ";
	    cin >> selection;
	    switch(selection)
 		{
			case 'H':
			case 'h':
			case 'F':
			case 'f':
				keepRunning = false;
				break;
			default:
				cout << endl << "Incorrect input, try again!";		
		}
		cout << endl;
	} while(keepRunning);

    // Agent Discovery
    agentDiscovery(MN, HA, FA, selection);
   
	// If mobile node is in foreign network
    if(selection == 'F' || selection == 'f')
    {
	    // Register Mobile Node to Home Agent
	    registerMN( MN, HA, FA );   

		// Display Menu for routing
		keepRunning = true;
		do {
			cout << "---------------------------------------------------------" << endl;
			cout << "                    Datagram Routing                     " << endl;
			cout << "---------------------------------------------------------" << endl;
			cout << "1. Indirect routing" << endl;
			cout << "2. Direct routing" << endl;
			cout << "Enter a selection: ";
			cin >> selection;
			switch(selection)
 			{
				case '1':
					indirectRouting(MN, HA, FA, CN);
					keepRunning = false;
					break;
				case '2':
					directRouting(MN, HA, FA, CN);
					keepRunning = false;
					break;
				default:
					cout << endl << "Incorrect input, try again!";		
			}
			cout << endl;
		} while(keepRunning);
    }
    return 0;
}

// Function Implementation
void Sleep(int time) { this_thread::sleep_for(chrono::seconds(time)); }

/*
This function generates a random IP address
*/
string generateIP()
{
   // Generate random IP address
   int ipInt[4] = { rand() % 31 + 192,
      rand() % 254 + 1,
      rand() % 254 + 1,
      rand() % 254 + 1};
   string IP = to_string(ipInt[0]);   
   for(int i = 1; i < 4; i++) IP += "." + to_string(ipInt[i]);
   
   // return the IP
   return IP;
}

/*
This function generates a random MAC address
*/
string generateMAC()
{
   // Generate random MAC address
   char buffer[33];
   string MAC = "";

   _itoa_s((rand() % 254 + 1), buffer, 4, 16);
   if (string(buffer).length() == 1)
	MAC += "0";
   MAC += buffer;

   for (int i = 1; i < 6; i++)
   {
   	_itoa_s((rand() % 254 + 1), buffer, 4, 16);
	MAC += "-";
	if (string(buffer).length() == 1)
		MAC += "0";
	MAC += buffer;
   }
  
   return MAC;
}

/*
This function simulates the agent discovery section of mobile IP. Its purpose is to 
discover home or foreign agents via agent advertisement. 

The first method is through advertisement, where an agent(home or foreign) periodically 
broadcasts an ICMP message specified with a type field of 9 (router discovery). The ICMP
message contains an IP address, home agent bit, foreign agent bit, registration required
bit (mobile node cannot get COA, for example with DHCP, without first registering), and
COA address fields.

The second method is through solicitation, where the mobile node broadcasts an ICMP message
specified with a type field of 10. When an agent receives the ICMP message, it unicasts an
agent advertisement to that mobile node.
*/
void agentDiscovery(mobileNode &m, homeAgent h, foreignAgent f, char homeOrForeign)
{
	// Select method (advertisement or solicitation)
	char selection;
	bool keepRunning = true;
	do {
		cout << "---------------------------------------------------------" << endl;
		cout << "                     Agent Discovery                     " << endl;
		cout << "---------------------------------------------------------" << endl;
		cout << "1. Advertisement" << endl;
		cout << "2. Solicitation" << endl;
		cout << "Enter agent discovery method: ";
		cin >> selection;
		switch(selection)
		{
			case '1':
			case '2':
				keepRunning = false;
				break;
			default:
				cout << endl << "Incorrect input, try again!";		
		}
		cout << endl;
	} while(keepRunning);

	// Solicitation
	if(selection == '2')
	{
		// Initialize ICMP solicitation message
		ICMP solicitation(SOLICITATION, m.getIP(), false, false, false);

		// Mobile node broadcast ICMP message to agent in network
		cout << "Mobile Node broadcasting solicitation..." << endl;
		solicitation.printICMP();
		Sleep(2);
	}

	// Advertisement
		// Listen for broadcast
		cout << "Mobile Node listening for Home Agent or Foreign Agent advertisement..." << endl << endl;
		Sleep(2);

		// Agent broadcast ICMP message/advertisement
		// If home network
		if(homeOrForeign == 'H' || homeOrForeign == 'h')
		{
			// Initialize ICMP advertisement message
			ICMP advertisement(ADVERTISEMENT, h.getHA(), true, false, false);
			advertisement.insertCOA(h.getHA());

			// Print advertisement
			if( selection == '2') cout << "Home Agent UNICASTING advertisement... " << endl;
			else cout << "Home Agent BROADCASTING advertisement... " << endl;
			advertisement.printICMP();
		}
		// Otherwise, foreign network
		else
		{
			// Initialize ICMP advertisement message
			ICMP advertisement(ADVERTISEMENT, f.getFA(), false, true, true);
			advertisement.insertCOA(f.getFA());
			
			// Print advertisement
			if( selection == '2') cout << "Foreign Agent UNICASTING advertisement... " << endl;
			else cout << "Foreign Agent BROADCASTING advertisement... " << endl;
			advertisement.printICMP();
		}
		Sleep(2);	
	

	// Check if function is not at home network	
	if( homeOrForeign == 'F' || homeOrForeign == 'f')
    {    
       // Confirm Mobile Node is in foreign network
       cout << "Mobile Node is in foreign network!" << endl << endl;
       Sleep(2);
	}

	// Otherwise, mobile IP is not needed
    else cout << "Mobile Node is in home network, no need for Mobile IP!" << endl;

	// Print divisor for next section
	cout << "---------------------------------------------------------" << endl << endl;
}

/*
This function registers a mobile node with its home agent when the mobile node is in a 
foreign network. Registration can be done by the foreign agent or directly by the mobile node.
The function below simulates registration by the foreign agent. There are four steps in
registration with the home agent:

	1. Mobile node sends registration message inside UDP datagram to port 434 to foreign 
	   agent. Message contains Care-of-Address of foreign agent, the address of the home 
	   agent, permanent IP address of the mobile node, the lifetime of the registration 
	   entry, and a 64-bit identification of the registration 
	   message (acts like sequence number to match received registration reply with request)
	2. Foreign agent records mobile node's permanent IP and sends registration message 
	   inside a UDP datagram to the home agent with encapsulation format
	3. Home agent checks registration's authenticity and correctness, binds mobile node's 
	   permanent IP with the care-of-address. Then sends a registration reply to 
	   the foreign agent containing the address of the home agent, address of the mobile
	   node, actual lifetime of the registration entry in the binding table, encapsulation
	   format and registration message identification
	4. Foreign agent receives the registration reply and forwards it to the 
	   mobile node. The message contains the address of the home agent, address of the mobile
	   node, lifetime of the registration entry, and registration message identification number

NOTE: There is no need to deregister care-of-address when a mobile node leaves a foreign 
network, because mobile node registering with a new foreign network takes care of this 
automatically when the mobile node registers a new care-of-address.
*/
void registerMN( mobileNode m, homeAgent h, foreignAgent f )
{
	// Display section title
	cout << "---------------------------------------------------------" << endl;
	cout << "                Registration with Home Agent             " << endl;
	cout << "---------------------------------------------------------" << endl;
         
	// Create registration lifetime and ID
	int lifetimeRequest = rand() % 800 + 199;
	int lifetimeReply = lifetimeRequest - rand() % 199;
	int registrationId = rand() % 999;

    // MN: send request to foreign agent
  	    // Set COA for mobile node
		m.setCOA(f.getFA());

		// Initialize registration REQUEST
		registrationMessage request(REQUEST, m.getCOA(), h.getHA(), m.getIP(), lifetimeRequest, registrationId);
	    cout << "Mobile Node: Sending registration request to Foreign Agent..." << endl;
		request.printRegistration(false);
		cout << endl;
	    Sleep(2);

    // FA: update visitor list
    cout << "Foreign Agent: Updating Visitor List..." << endl << endl;
    Sleep(2);
    f.addEntry(m.getIP(), h.getHA(), m.getMAC(), lifetimeRequest);
    f.printEntries();
    cout << endl << "Visitor List is updated!" << endl << endl << endl;
    Sleep(3);

    // FA: forward request to home agent
    cout << "Foreign Agent: Forwarding registration request to Home Agent..." << endl;
    request.printRegistration(true);
	cout << endl;
	Sleep(2);
            
    // HA: update binding table
    cout << "Home Agent: Updated Mobile Binding Table..." << endl << endl;
    Sleep(2);
    h.addEntry(m.getIP(), f.getFA(), lifetimeReply);
    h.printEntries();
    cout << endl << "Mobile Binding Table is updated!" << endl << endl << endl;
    Sleep(3);

	// HA: send reply to foreign agent
		// Initialize registration REPLY
		registrationMessage reply(REPLY, "", h.getHA(), m.getIP(), lifetimeReply, registrationId);
		cout << "Home Agent: Sending registration reply to Foreign Agent..." << endl;
		reply.printRegistration(true);
		cout << endl;
		Sleep(2);
            
    // FA: relay reply to mobile node
    cout << "Foreign Agent: Forwarding registration reply to Mobile Node..." << endl;
	reply.printRegistration(false);
    Sleep(2);   

	// Print divisor for next section
	cout << "---------------------------------------------------------" << endl << endl;
}

/*
The correspondent node sends datagrams addressed to the mobile nodes permanent IP address.
The datagrams are routed to the mobile node's home network, where the home agent will 
intercept the datagrams. It forwards the encapsulated datagrams (tunneling) to the foreign 
agent of the mobile node specified in its binding table. The foreign agent then forwards the 
decapsulated datagrams to the mobile node. The correspondent node is unaware that the mobile 
node is located in a foreign network.
*/
void indirectRouting(mobileNode MN,homeAgent HA, foreignAgent FA,correspondentNode CN)
{
	// Display section title
	cout << "---------------------------------------------------------" << endl;
	cout << "               Indirect Routing of Datagrams             " << endl;
	cout << "---------------------------------------------------------" << endl;
  
	// Send datagram from correspondent node to home agent
	cout << "Sending packet from Mobile Node to Foreign Agent" << endl << endl;

	datagram originalDatagram(MN.getIP(), FA.getFA(), 15, rand() % 65536);

	// print and update datagram
	originalDatagram.print();
	originalDatagram.decreaseTTL();

	// Encapsulate orignal datagram and send to Home Agent
	cout << "Foreign Agent recieved packet, sending encapsulated packet to Home Agent" << endl << endl;

	datagram encapsDatagram(FA.getFA(), HA.getHA(), 15, rand() % 65536);

	// print information
	encapsDatagram.print();

	// send from Home Agent to Correspondent Node
	cout << "Home Agent recieved packet, sending original packet to Correspondent Node" << endl << endl;

	// print information
	originalDatagram.print();

	// packet arrived
	cout << "Correspondent Node recieved packet" << endl << endl;

	// Print divisor for next section
	cout << "---------------------------------------------------------" << endl << endl;
}

/*
Direct routing solves the triangle routing problem, which is the inefficiency of sending
datagrams to the home agent if the mobile node and correspondent node are in close proximity.
With direct routing, the correspondent node must first learn the care-of-address of the
mobile node at the beginning of a session. This is done with a query by the correspondent 
agent to the home agent. After this initial query, the correspondent node can directly
tunnel datagrams to the mobile node's care-of-address. With this method, a foreign anchor
agent is needed, since the mobile node's care-of-address is only queried once. The first
foreign agent serves as the foreign anchor agent. This anchor is needed if the mobile node 
moves to a different foreign network. Once the mobile node moves to a new foreign network,
the foreign agent of that network sends the foreign anchor agent the mobile node's new 
care-of-address. The correspondent node tunnels datagrams addressed to the foreign anchor 
agent, who then forwards those datagrams to the mobile node's new foreign agent.
*/
void directRouting(mobileNode MN,homeAgent HA, foreignAgent FA,correspondentNode CN)
{
	// Display section title
	cout << "---------------------------------------------------------" << endl;
	cout << "                Direct Routing of Datagrams              " << endl;
	cout << "---------------------------------------------------------" << endl;
  
	// send Datagram directly to Correspondent Node using permanent home address

	cout << "Mobile Node sending datagram to Correspondent Node using permanent home address" << endl << endl;

	datagram newDatagram(MN.getIP(), CN.getIP(), 15, rand() % 65536);

	newDatagram.print();

	// Print divisor for next section
	cout << "---------------------------------------------------------" << endl << endl;
}