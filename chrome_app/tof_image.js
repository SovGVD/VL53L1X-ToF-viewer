const DEVICE_PATH = '/dev/ttyUSB0';

const ZOOM = 30;

var image = document.getElementById('image');
var ctx = image.getContext("2d");

var buffer = "";

var tof = new Array(16).fill(0).map(() => new Array(16).fill(0));
var tofId = new Array(16).fill(0).map(() => new Array(16).fill(0));

function distanceToColor(distance)
{
	let maxd = 0;
	let mind = 900000;
	for (var x = 0; x < 16; x++)
	{
		for (var y = 0; y < 16; y++) {
			if (tof[x][y] != -1) {
				if (tof[x][y] > maxd) maxd = tof[x][y];
				if (tof[x][y] < mind) mind = tof[x][y];
			}
		}
	}
	
	let norm = (distance - mind)/(maxd - mind);
	
	return (1-norm)*255;
}


function toBuffer(raw)
{
	buffer += raw;
	
	console.log('BUF', buffer);
	
	if (idx = buffer.indexOf("\n") !== -1) {
		tmp = buffer.split("\n");
		buffer = tmp.pop();
		for (var i = 0; i < tmp.length; i++){
			parseTOFString(tmp[i]);
		}
	}
}

function drawImage() {
	for (var x = 0; x < 16; x++)
	{
		for (var y = 0; y < 16; y++) {
			if (tof[x][y] == -1) {
				ctx.fillStyle = "rgba(255,0,0,255)";
			} else {
				let c = distanceToColor(tof[x][y]);
				ctx.fillStyle = "rgba("+c+","+c+","+c+",255)";
			}
			ctx.fillRect( x*ZOOM, y*ZOOM, ZOOM, ZOOM);
			
			ctx.font = "8px Arial";
			ctx.fillStyle = "rgba(0,255,0,128)";
			ctx.fillText(tof[x][y], x*ZOOM+5, y*ZOOM+10);
			ctx.fillText(tofId[x][y], x*ZOOM+5, y*ZOOM+20);
		}
	}
	
}

function parseTOFString(s)
{
	var s = s.split("	");
	
	let x = parseInt(s[1]);
	let y = parseInt(s[2]);
	let d = parseInt(s[3]);
	
	tof[x][y] = d;
	tofId[x][y] = parseInt(s[0]);
	
	drawImage();
	
	
	
}

var device = new serial({serialPort : DEVICE_PATH, baudRate : 115200, parser : "\n", connectImmediately : true})


