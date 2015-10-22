public class HotelImpl extends java.rmi.server.UnicastRemoteObject implements Hotel{
	private HotelRoomType[] rooms;
	private int totalCapacity;

	public HotelImpl() throws java.rmi.RemoteException{
		int[] capacities = {10, 20, 20};

		rooms = new HotelRoomType[capacities.length];

		for(int i = 0; i < rooms.length; i++){
			rooms[i] = new HotelRoomType(capacities[i]);
			totalCapacity += capacities[i];
		}
	} 

	public int[] listAvailableRooms() throws java.rmi.RemoteException{
		int[] result = new int[rooms.length];

		for(int i = 0; i < rooms.length; i++){
			result[i] = rooms[i].getNumberAvailableRooms();
		}

		return result;		
	} 

	public boolean bookRoom(int typeNumber, String guestName) throws java.rmi.RemoteException{
		if(typeNumber <= 0 || typeNumber > rooms.length){
			return false;
		}

		return rooms[typeNumber-1].book(guestName);
	} 

	public String[] listGuests() throws java.rmi.RemoteException{
		String[] result;
		int offset = 0;
		int resultSize = 0; 

		//compute length of result array
		for(int i = 0; i < rooms.length; i++){
			resultSize += rooms[i].getNumberBookedRooms();
		}

		//create result array
		result = new String[resultSize];

		//fill result array
		for(int i = 0; i < rooms.length; i++){
			System.arraycopy(rooms[i].getGuestNames(), 0, result, offset, rooms[i].getNumberBookedRooms());
			offset += rooms[i].getNumberBookedRooms();
		}

		return result;		
	}
}
