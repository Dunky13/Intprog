import java.io.PrintWriter;

abstract public class HotelDisplayLogic{
	protected HotelInterface hotelObject;

	protected void performAction(PrintWriter out, int action, int roomType, String guestName){
		if(action == 0){
			printHelp(out);
			return;
		}

		try{
			if(action == 1){
				listAvailableRooms(out);
			}
			else if(action == 2){
				bookRoom(out, roomType, guestName);
			}
			else if(action == 3){
				listGuests(out);
			}
		} 
		catch (Exception e){
			out.println("Received Exception:"); 
			e.printStackTrace(out);
		}
	}

	private void listAvailableRooms(PrintWriter out) throws java.rmi.RemoteException{
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

	private void bookRoom(PrintWriter out, int roomType, String guestName) throws java.rmi.RemoteException{
		if(hotelObject.bookRoom(roomType, guestName)){
			out.println("ok");
		}
		else{
			out.println("NA");			
		}
	}

	private void listGuests(PrintWriter out) throws java.rmi.RemoteException{
		int i;
		String[] guestList = hotelObject.listGuests();
		
		for(i = 0; i < guestList.length; i++){
			out.printf("%s\n", guestList[i]);
		}
	}

	abstract protected void printHelp(PrintWriter out);
}