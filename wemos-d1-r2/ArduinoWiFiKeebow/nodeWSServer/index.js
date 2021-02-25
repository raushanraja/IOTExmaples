const socket = require('socket.io')();
const { exec } = require('child_process');


 


socket.on('connection',client =>{  
    console.log("Connected", ' client id:', client.id);
    client.emit('init',{message:'connected'})
    client.on('message', message => handleMessage(message,client));
})


function handleMessage(message,client){
    console.log(`${message.pavu}\r`);
    if(message.pavu ==1){
      try{
        let x =  exec('pidof pavucontrol', function(err, out, code) {
            if(out){
                exec(`kill ${out}`,(err,out,code)=>{
                    console.log("killed\r");
                })
            }
            else{
                exec(`pavucontrol`)
            }
       });
      }
      catch(e){
        console.log(e);
      }
    }
    // socket.sockets.emit('received',{message:message.pavu,sender:client.id,type:'received'})
}


socket.listen(3000);
