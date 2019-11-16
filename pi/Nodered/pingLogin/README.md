+ Ping and Login

+++ Pings google > if ok then sends local ip notification to pushover and stop else 
+++ ping local network > if ok then login to potal then sends push notification to pushover and stop else
+++ pings local network every 2 seconds if network available then go to step 2
+++ also check for Wifi device, if not connected then stops on first run
