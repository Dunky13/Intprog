import java.net.MalformedURLException; 
import java.rmi.Naming;
import gnu.getopt.*;
import java.io.PrintWriter;
import java.lang.StringBuffer;

public class HotelClient extends HotelDisplayLogic{
	private PrintWriter out;

	private void makeHotelObject(String serverAddress){
		try{
		    hotelObject = (HotelInterface) Naming.lookup("rmi://" + serverAddress + "/HotelService");
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

		if(action == -1 || (indexFirstNonOption = g.getOptind()) == 0){	/*error*/
			out.println("See HotelClient or HotelClient -h for usage");
		}
		else{
			/*Get values from non option arguments (server address, guest name in case of booking */
			serverAddress = args[indexFirstNonOption];

			if(action == 2){	//get name at booking room
				getGuestName(args, indexFirstNonOption + 1, guestName);
			}

			makeHotelObject(serverAddress);

			/*Perform request*/
			performAction(out, action, roomType, guestName.toString());
		}

		/*Cleanup*/
		out.flush();
		out.close();
	}

	//validate and obtain room number
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

	//validate and obtain guest name
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

	protected void printHelp(PrintWriter out){
		String help = "This program can be used by the attendees of a conference to make room reservations. Reservation dates are fixed, so book with care.\n" +
			"\n" +
			"Usage:\n" +
			"	Help: java HotelClient or java HotelClient -h\n" +
			"	List available rooms per type: java HotelClient <server address> -l\n" +
			"	Book a room: java HotelClient <server address> -b <type> 'Guest Name'\n" +
			"	List guests: java HotelClient <server address> -g\n";

		out.printf("%s", help);		
	}

	public static void main(String[] args){
		new HotelClient().start(args);
	}
}
