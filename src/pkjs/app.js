
var MAX_CHUNK_SIZE = 8000;  // From app_message_inbox_size_maximum()

var URL="http://pebble-insta.tk";
var TOKEN="q9cRBCrWmxjWq4xlNqKetAtcyhkTyajh";
var NUMB=20;

function sendChunk(array, index, arrayLength) {
  // Determine the next chunk size
  var chunkSize;
  if(arrayLength - index < MAX_CHUNK_SIZE) {
    // Will only need one more chunk
    chunkSize = arrayLength - index;
  } else {
    // Will require multiple chunks for remaining data
    chunkSize = MAX_CHUNK_SIZE;
  }

  // Prepare the dictionary
  var dict = {
    'chunk': array.slice(index, index + chunkSize),
    'chunksize': chunkSize,
    'index': index
  };

  // Send the chunk
  Pebble.sendAppMessage(dict, function() {
    // Success
    index += chunkSize;

    if(index < arrayLength) {
      // Send the next chunk
      sendChunk(array, index, arrayLength);
    } else {
      // Complete!
      Pebble.sendAppMessage({'complete': 0});
    }
  }, function(e) {
    console.log('Failed to send chunk with index ' + index);
  });
}

function transmitImage(array) {
  var index = 0;
  var arrayLength = array.length;
  // Transmit the length for array allocation
  Pebble.sendAppMessage({'lenght': arrayLength}, function(e) {
    // Success, begin sending chunks
    sendChunk(array, index, arrayLength);
  }, function(e) {
    console.log('Failed to send data length to Pebble!');
  });
}

function processImage(responseData) {
  // Convert to a array
  var byteArray = new Uint8Array(responseData);
  var array = [];
  for(var i = 0; i < byteArray.byteLength; i++) {
    array.push(byteArray[i]);
  }

  // Send chunks to Pebble
  transmitImage(array);
}



function downloadImage(i) {


  var request = new XMLHttpRequest();
  request.onload = function() {
    processImage(this.response);
  };
  request.responseType = "arraybuffer";
  request.open("GET", URL+"/data/"+TOKEN+'/'+parseInt(i));
  request.send();
}

function sendCard(numb){
  var uname=JSON.parse(localStorage.getItem('names'))[numb];
  Pebble.sendAppMessage({'username': uname});
  var like=JSON.parse(localStorage.getItem('ulikes'))[numb]; 
  if (like) {
    Pebble.sendAppMessage({'is_like': 1});
  }
  else{
    Pebble.sendAppMessage({'is_like':2});    
  }
  
  var likes=JSON.parse(localStorage.getItem('likes'))[numb];
  Pebble.sendAppMessage({'likes':likes});
  
  var dates=JSON.parse(localStorage.getItem('dates'))[numb];
  Pebble.sendAppMessage({'dates':dates}); 

  var texts=JSON.parse(localStorage.getItem('texts'))[numb];
  Pebble.sendAppMessage({'captions':texts}); 
  
  downloadImage(numb);

}






function downloadjson() {

  var request = new XMLHttpRequest();
  request.onload = function() {
    var resp=this.response;
    if (!resp.all)
      {
        Pebble.sendAppMessage({'state': 2});
        return;
      }
    
    localStorage.setItem('likes', JSON.stringify(resp.likes));
    localStorage.setItem('names', JSON.stringify(resp.names));
    localStorage.setItem('texts', JSON.stringify(resp.texts)); 
    localStorage.setItem('lids', JSON.stringify(resp.lids)); 
    localStorage.setItem('ulikes', JSON.stringify(resp.ulikes)); 
    localStorage.setItem('dates', JSON.stringify(resp.dates)); 
    Pebble.sendAppMessage({'pages': resp.all});

    Pebble.sendAppMessage({'state': 1});
  };
  request.responseType = "json";
  request.open("GET", URL+'/get_json/'+TOKEN+'/'+parseInt(NUMB));
  request.send();
}



Pebble.addEventListener('appmessage', function(e) {
  // Get the dictionary from the message
  var d = e.payload;

  if(d.hasOwnProperty('update')) {
    console.log('is update!'+d.update);
  // The RequestData key is present, read the value
  sendCard(d.update);
}
   if(d.hasOwnProperty('sendlike')) {
    var lid=JSON.parse(localStorage.getItem('lids'))[parseInt(d.sendlike)];
    var request = new XMLHttpRequest();
    request.open("GET", URL+'/like/'+TOKEN+'/'+lid);
    request.send();  
    var a=JSON.parse(localStorage.getItem('likes'));
    a[parseInt(d.sendlike)]++;
    localStorage.setItem('likes', JSON.stringify(a));
    
    a=JSON.parse(localStorage.getItem('ulikes'));
    a[parseInt(d.sendlike)]=true;
    localStorage.setItem('ulikes', JSON.stringify(a));    
  // The RequestData key is present, read the value
  
} 
  
  
  
  
});

Pebble.addEventListener('ready', function() {
  downloadjson();
  
  
});