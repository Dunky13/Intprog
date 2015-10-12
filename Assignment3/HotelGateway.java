import java.net.MalformedURLException; 
import java.rmi.Naming;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.InputStreamReader;

public class HotelGateway extends HotelDisplayLogic{
	static final int PORT = 3333;

	private ServerSocket serverSocket;
    private Socket clientSocket;  
    private BufferedReader in;
    private boolean connectionOpened;

	public HotelGateway(){
		connectionOpened = false;
	}

	private void makeHotelObject(String serverAddress){
		try{
		    hotelObject = (Hotel) Naming.lookup("rmi://" + serverAddress + "/HotelService");
		} 
		catch (Exception e){
			out.println("Received Exception:");
			e.printStackTrace();
		}
	}

	private void processRequest(){
		String[] input = new String[1];
		BufferedReader in;

		try{
			if(!connectionOpened){
				serverSocket = new ServerSocket(PORT);
				clientSocket = serverSocket.accept();

			    out = new PrintWriter(clientSocket.getOutputStream(), true);

			    connectionOpened = true;
			}

			out.print("hotelgw>");
			out.flush();

		    in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));			

			input = in.readLine().split("\\s+", 3);

			if(input[0].equals("")){
				return;
			}
		}
		catch(IOException e){
			e.printStackTrace();
		}

		parseAndProcessInput(input);
	}

	private void parseAndProcessInput(String[] input){
		int action = 0;
		int roomType = 0;
		String guestName = "";
		char command = input[0].charAt(0);

		switch(command){
			case 'l':
				action = 1;
			break;
			case 'b':
				if(input.length != 3){
					action = -1;
					out.println("Booking requires a room type and a guest name");
				}
				else{
					roomType = Integer.parseInt(input[1]);
					guestName = input[2];
					action = 2;
				}

			break;
			case 'g':
				action = 3;
			break;
			case 'q':
				action = 4;
			break;
			case 'h':
			default:
			break;
		}
	
		if(action == 4){
			try{
				//in.close();
				out.flush();
				out.close();
				serverSocket.close();
				clientSocket.close();
				connectionOpened = false;
			}
			catch(IOException e){
				e.printStackTrace();
			}
		}
		else if(action >= 0){
		    performAction(action, roomType, guestName);
		}
	}

	protected void printHelp(){
		String help = "This program can be used by the attendees of a conference to make room reservations. Reservation dates are fixed, so book with care.\n" +
			"\n" +
			"Usage\n" +
			"\n" +
			"Help:\n" +
			"	h\n" +
			" 	Displays this help message.\n" +
			"List available rooms:\n" +
			" 	l\n" +
			" 	Lists the number of rooms available in each price range.\n" +
			"Book a room:\n" +
			"	b <type> Guest Name\n" +
			"	Books a room of the specified type (if available) on the given name.\n" +
			"List guests:\n" +
			"	g\n" +
			"	Lists the names of all registered guests.\n" +
			"Quit:\n"
			"	q\n" +
			"	Quits the telnet session.\n";

		out.printf("%s", help);
	}

	private void start(String serverAddress){
		makeHotelObject(serverAddress);
		
		while(true){
			processRequest();
		}
	}

	public static void main(String[] args){
		if(args.length != 1){
			System.out.println("Usage: java HotelGateway <server address>");
		}
		else{
			new HotelGateway().start(args[0]);
		}
	}
}