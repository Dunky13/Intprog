<?php 

class HotelGatewayConnection{
	private $socket;
	const BUFFER_SIZE = 2048;
	const PROMPT = 'hotelgw>';

	function __construct($address, $port){
		$address = gethostbyname($address);

		/* Create a TCP/IP socket. */
		$this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		
		if($this->socket === false){
			echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
			return;
		}

		$result = socket_connect($this->socket, $address, $port);

		if ($result === false){
			echo "socket_connect() failed.\nReason: ($result) " . socket_strerror(socket_last_error($this->socket)) . "\n";
			return;
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

		$this->readPrompt();

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
			echo "socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
		}
		elseif($buf == self::PROMPT){
			socket_read($this->socket, $bytes);
			return true;
		}

		return false;
	}

	public function __destroy(){
		if($this->socket !== false){
			socket_close($this->socket);
		}
	}
}