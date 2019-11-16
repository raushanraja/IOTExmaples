+ Ping and Login
+ Pings google > if ok then sends local ip notification to pushover and stop,  else 
+ Ping local network > if ok then login to potal then sends push notification to pushover and stop,  else
+ Pings local network every 2 seconds if network available then go to step 2
+ Also check for Wifi device, if not connected then stops on first run