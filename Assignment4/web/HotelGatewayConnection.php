<?php 

class HotelGatewayConnection{
	private $socket;
	const BUFFER_SIZE = 2048;
	const PROMPT = 'hotelgw>';

	function __construct($address, $port){
		$address = gethostbyname($address);

		/* Create a TCP/IP socket. */
		$this->socket = @socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		
		if($this->socket === false){
			throw new Exception('socket_create() failed. Reason: ' . socket_strerror(socket_last_error()));
		}

		$result = @socket_connect($this->socket, $address, $port);

		if ($result === false){
			throw new Exception('socket_connect() failed. Reason: (' . $result . ') = ' . socket_strerror(socket_last_error($this->socket)));
		}
	}

	public function request($command){
		if($this->socket === false){
			return 'Error: no socket';
		}

		$command .= "\n";

		socket_write($this->socket, $command, strlen($command));

		$result = '';
		$buf = '';

		try{
			if($this->readPrompt() == 2){		//empty response, for example no guests in list
				return $result;
			}
		}
		catch(Exception $e){
			return $e->getMessage();
		}

		while($out = socket_read($this->socket, self::BUFFER_SIZE, PHP_NORMAL_READ)){
			$result .= $out;

			if($this->readPrompt()){
				break;
			}
		}

		return $result;
	}

	private function readPrompt(){
		if(false === ($bytes = socket_recv($this->socket, $buf, self::BUFFER_SIZE, MSG_PEEK))){
			throw new Exception("socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)));
		}
		if($buf == self::PROMPT){
			socket_read($this->socket, $bytes);
			return 1;
		}

		return 0;
	}

	public function __destroy(){
		if($this->socket !== false){
			socket_close($this->socket);
		}
	}
}