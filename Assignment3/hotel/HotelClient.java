import java.net.MalformedURLException; 
import java.rmi.Naming;
import gnu.getopt.*;
import java.io.PrintWriter;
import java.lang.StringBuffer;

public class HotelClient extends HotelDisplayLogic{
	private void makeHotelObject(String serverAddress){
		try{
		    hotelObject = (Hotel) Naming.lookup("rmi://" + serverAddress + "/HotelService");
		} 
		catch (Exception e){
			out.println("Received Exception:");
			e.printStackTrace();
		}
	}

	private void start(String[] args){
		int c, indexFirstNonOption;
		String serverAddress;

		int action = 0;
		int roomType = 0;
		StringBuffer guestName = new StringBuffer();

		out = new PrintWriter(System.out);

		/*Parse command line options*/
		Getopt g = new Getopt("HotelClient", args, "lb:gh");

		while((c = g.getopt()) != -1){
			switch(c){
				case 'l':
					action = 1;
				break;
				case 'b':
					action = 2;
					roomType = getRoomType(g);
				break;
				case 'g':
					action = 3;
				break;
				case 'h':
				break;
				case '?':		//error. getopt() already printed an error
				default:
					action = -1;
				break;
			}
		}

		/*Get values from non option arguments (server address, guest name in case of booking */
		if((indexFirstNonOption = g.getOptind()) < 0){
			out.println("Please provide a server address");
		}

		serverAddress = args[indexFirstNonOption];

		if(action == 2){	//get name at booking room
			getGuestName(args, indexFirstNonOption + 1, guestName);
		}

		/*Perform request*/
		makeHotelObject(serverAddress);

		if(action == -1){	/*error*/
			out.println("See HotelClient or HotelClient -h for usage");
		}
		else{
			performAction(action, roomType, guestName.toString());
		}

		/*Cleanup*/
		out.flush();
		out.close();
	}

	private int getRoomType(Getopt g){
		String arg = g.getOptarg();

		if(!arg.matches("\\d+")){
			out.println("Please provide a positive integer as room number after the -b option");
			out.flush();
			out.close();
			System.exit(-1);
		}

		return Integer.parseInt(arg);
	}

	private void getGuestName(String[] args, int start, StringBuffer guestName){
		int i;
		boolean first = true;

		if(args.length > start){
			for(i = start; i < args.length; i++){
				if(first){
					first = false;
				}
				else{
					guestName.append(' ');
				}

				guestName.append(args[i]);
			}
		}
		else{
			out.println("Please provide a name");
			out.flush();
			out.close();
			System.exit(-1);
		}
	}

	protected void printHelp(){
		String help = "This program can be used by the attendees of a conference to make room reservations. Reservation dates are fixed, so book with care.\n" +
			"\n" +
			"Usage\n" +
			"\n" +
			"Help:\n" +
			"	$ java HotelClient or $ java HotelClient -h\n" +
			" 	Displays this help message.\n" +
			"List available rooms:\n" +
			" 	$ java HotelClient <server address> -l\n" +
			" 	Lists the number of rooms available in each price range.\n" +
			"Book a room:\n" +
			"	$ java HotelClient <server address> -b <type> 'Guest Name'\n" +
			"	Books a room of the specified type (if available) on the given name.\n" +
			"List guests:\n" +
			"	$ java HotelClient <server address> -g\n" +
			"	Lists the names of all registered guests.\n";

		out.printf("%s", help);		
	}

	public static void main(String[] args){
		new HotelClient().start(args);
	}
}
