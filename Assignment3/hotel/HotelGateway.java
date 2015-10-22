import java.net.MalformedURLException; 
import java.rmi.Naming;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;

public class HotelGateway{
	static final int PORT = 3333;

	private Hotel hotelObject;

	public HotelGateway(){

	}

	private void makeHotelObject(String serverAddress){
		try{
		    hotelObject = (Hotel) Naming.lookup("rmi://" + serverAddress + "/HotelService");
		} 
		catch (Exception e){
			System.out.println("Received Exception:");
			e.printStackTrace();
		}
	}

	private void start(String serverAddress){
		ServerSocket serverSocket;
		Socket clientSocket = null;

		makeHotelObject(serverAddress);
	    
	    try{
			serverSocket = new ServerSocket(PORT);

			while(true){
				clientSocket = serverSocket.accept();

				new Thread(new HotelGatewayThread(clientSocket, hotelObject)).start();
			}
		}
		catch(IOException e){
			e.printStackTrace();
			return;
		}

		//serverSocket.close();
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