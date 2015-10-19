import java.io.PrintWriter;

abstract public class HotelDisplayLogic{
	protected Hotel hotelObject;
	protected PrintWriter out;

	protected void performAction(int action, int roomType, String guestName){
		if(action == 0){
			printHelp();
			return;
		}

		try{
			if(action == 1){
				listAvailableRooms();
			}
			else if(action == 2){
				bookRoom(roomType, guestName);
			}
			else if(action == 3){
				listGuests();
			}
		} 
		catch (Exception e){
			out.println("Received Exception:"); 
			e.printStackTrace(out);
		}
	}

	protected void listAvailableRooms() throws java.rmi.RemoteException{
		int[] availableRooms;
		int i;

		availableRooms = hotelObject.listAvailableRooms();

		for(i = 0; i < availableRooms.length; i++){
			if(i > 0){
				out.printf("\t");
			}

			out.printf("%d", availableRooms[i]);
		}

		out.println();
	}

	protected void bookRoom(int roomType, String guestName) throws java.rmi.RemoteException{
		if(hotelObject.bookRoom(roomType, guestName)){
			out.println("ok");
		}
		else{
			out.println("NA");			
		}
	}

	protected void listGuests() throws java.rmi.RemoteException{
		int i;
		String[] guestList = hotelObject.listGuests();
		
		for(i = 0; i < guestList.length; i++){
			out.printf("%s\n", guestList[i]);
		}
	}

	abstract protected void printHelp();
}