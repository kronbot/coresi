const express = require("express");
const port = 8000;
const app = express();
const raspi = require("raspi");
const serial = require("serialport").SerialPort;


const main = async () => {
	const response = await serial.list();
	console.log(response);
	
//	if (response[0].serialNumber === 385fe88d999cec11a324614d73138bba) {
		const portESPServo = new serial({baudRate: 115200, path: response[0].path});
//		const portESPStepper = new serial({baudRate: 115200, path: response[1].path}); 
//	} else {
//		const portESPServo = new serial({baudRate: 115200, path: response[1].path});
//		const portESPStepper = new serial({baudRate: 115200, path: response[0].path}); 
//	}

	const servoData = "", stepperData = "";

	portESPServo.on("data", (data) => { 
		servoData = Buffer.from(data).toString("utf8"); 
		console.log("Recieved Servo: " + servoData); 
	});
//  portESPStepper.on("data", (data) => { 
//		stepperData Buffer.from(data).toString("utf8")); 
//		console.log("Recieved Stepper: " + stepperData); 
//	});

//	console.log(Object.getOwnPropertyNames(raspi));
//	console.log(Object.getOwnPropertyNames(serial));

	app.get("/servo", (request, response) => {response.json({message: servoData})});
//	app.get("/stepper", (request, response) => {response.json({message: stepperData})});
	
	app.post("/servo/:command", (request, response) => {
		const command = request.params.command;
		portESPServo.write(command);
		response.json({message: "succes"});
		console.log("Send Servo: " + command);
	});
//	app.post("/stepper/:command", (request, response) => {
//		const command = request.params.command;
//		portESPStepper.write(command);
//		response.json({message: "succes"});
//		console.log("Send Stepper: " + command);
//	});

	app.listen(port, () => {console.log("running")});
}

main();
