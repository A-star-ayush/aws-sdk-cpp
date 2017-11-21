window.onload = function(){

	var canvas = document.getElementById("canvas");
	var ct = canvas.getContext("2d");
	canvas.height = window.innerHeight;
	canvas.width = window.innerWidth;
	ct.fillStyle="#BEBEBE";

	var radius = 30;  // radius for the circles representing the vertices

	var vFrom = 0;  // the source vertex for an edge

	var v = 0;
	var vertices = [];
	var adjList = [];

	canvas.onmousedown = EorV;
	window.onkeypress = keyPressed;

	function EorV(e){
		if(e.button==0){  // should work only for left click button
			var x1 = e.offsetX;
			var y1 = e.offsetY;
			var r2 = radius*radius;
			for(var i=0;i<vertices.length;++i){
				var x2 = vertices[i].x;
				var y2 = vertices[i].y;
				if((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) < r2){
					vFrom = i;
					canvas.onmouseup = destinationV;
					break;
				}
			}
	
			if(i==vertices.length){
				adjList[v] = [];
				vertices.push({ id: v++, x : x1, y : y1 });
				ct.beginPath();
				ct.arc(x1, y1, radius, 0, 2*Math.PI);
				ct.fill();
			}
		}
	}

	function destinationV(e){
		var x1 = e.offsetX;
		var y1 = e.offsetY;
		var r2 = radius*radius;
		for(var i=0;i<vertices.length;++i){
			var x2 = vertices[i].x;
			var y2 = vertices[i].y;
			if((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) < r2){
				var id1 = vertices[vFrom].id; 
				var id2 = vertices[i].id;
				var parent, child;
				parent = id2^((id1^id2) &  -(id1 > id2));  // parent is the one with the bigger id
				child = id2^((id1^id2) &  -(id1 < id2));	
				adjList[child].push(parent);
				ct.beginPath();
				var dir = (vertices[i].x < vertices[vFrom].x) ? -1 : 1; 
				var theta = Math.atan((vertices[i].y - vertices[vFrom].y)/(vertices[i].x - vertices[vFrom].x));
				ct.moveTo(vertices[vFrom].x + dir*radius*Math.cos(theta), vertices[vFrom].y + dir*radius*Math.sin(theta));
				ct.lineTo(vertices[i].x - dir*radius*Math.cos(theta), vertices[i].y - dir*radius*Math.sin(theta));
				ct.stroke();
				break;
			}
		}

		if(i==vertices.length) { alert("Edge not drawn: no destination found"); }
		canvas.onmouseup = null;
	}
	
	function keyPressed(e){
		var x = e.which || e.keyCode;
		if(x==13) logAdjList();
		else drawColoredGraph();
	}
	function logAdjList(){
		 var output = adjList.length + " ";
		 for(var i=0;i<adjList.length;++i){
		 	output = output + adjList[i].length + " " + adjList[i].join(" ") + " ";
		 }
		 alert(output);
	}

	function drawColoredGraph(){
		ct.clearRect(0, 0, canvas.width, canvas.height);

		var file = document.getElementById("file").files[0];
		var reader = new FileReader();

		reader.onloadend = function(e) {
			if (e.target.readyState == FileReader.DONE){
				var colors = e.target.result;
				console.log(colors);
				colors = colors.split(" ");
				console.log(colors);
				for(var i=0;i<colors.length;++i)
					colors[i] = parseInt(colors[i]);
				
				var colorFill = ["#E42A2A", "#785C5C", "#D77C15", "#D8D61F", "#4EA1DD", "#B20C87", "#77E925",
						 	     "#30EA99", "#000000", "#4A1FB0"];

				for(var i=0;i<vertices.length;++i){
					ct.beginPath();
					ct.arc(vertices[i].x, vertices[i].y, radius, 0, 2*Math.PI);
					ct.fillStyle = colorFill[colors[i]];
					ct.fill();
				}

				for(var i=0;i<adjList.length;++i){
					for(var j=0;j<adjList[i].length;++j){
						ct.beginPath();
						var dir = (vertices[adjList[i][j]].x < vertices[i].x) ? -1 : 1; 
						var theta = Math.atan((vertices[adjList[i][j]].y - vertices[i].y)/(vertices[adjList[i][j]].x - vertices[i].x));
						ct.moveTo(vertices[i].x + dir*radius*Math.cos(theta), vertices[i].y + dir*radius*Math.sin(theta));
						ct.lineTo(vertices[adjList[i][j]].x - dir*radius*Math.cos(theta), vertices[adjList[i][j]].y - dir*radius*Math.sin(theta));
						ct.stroke();
					}
				}
			}
		};

		reader.readAsBinaryString(file);
	}
};