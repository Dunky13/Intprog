class HotelRoomType{
	private int capacity;
	private String[] guestNames;
	private int numberBookedRooms; 

	public HotelRoomType(int capacity){
		this.capacity = capacity;
		this.guestNames = new String[capacity];
		numberBookedRooms = 0;
	}

	public boolean fullyBooked(){
		return numberBookedRooms == capacity;
	}

	public boolean book(String guestName){
		if(fullyBooked()){
			return false;
		}
		else{
			guestNames[numberBookedRooms] = guestName;
			numberBookedRooms++;
			return true;
		}
	}

	public int getNumberBookedRooms(){
		return numberBookedRooms;
	}

	public int getNumberAvailableRooms(){
		return capacity - numberBookedRooms;
	}

	public String[] getGuestNames(){
		return guestNames;
	}
}