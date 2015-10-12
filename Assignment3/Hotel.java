public interface Hotel extends java.rmi.Remote
{ 
	public int[] listAvailableRooms() throws java.rmi.RemoteException; 

	public boolean bookRoom(int typeNumber, String guestName) throws java.rmi.RemoteException; 

	public String[] listGuests() throws java.rmi.RemoteException; 
} 
