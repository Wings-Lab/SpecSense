# SpectrumSensingWithNotifications

Kaa-sandbox server available at: 130.245.144.129:9080/sandbox

To start the application, first make sure the above sandbox server is running.
If not, start it on the VirtualBox installed on the Redbox.

1. Once it starts, go to the endpoint (odroid, raspberry pi or desktop)
2. If there are any changes in the source file NotificationDemo_with_actual_sampling.cpp, go to the build directory and run 'make -j4' to build the SDK
3. This will build the SDK and generate an executable kaa-app
4. If this is a test to calculate the data rate and if the number of samples sent is important, then drop the mongodb database before executing the executable by running drop_db.py in analysis_files directory
4. Run this executable (./kaa-app) to start collecting and sending the samples back to the server on the kaa sandbox
5. Once the test is complete, run data_rate_computation.py in analysis_files directory to get the number of samples in the database

Steps to create visualizations on MongoDB Compass for SpecSense data:

1. Download MongoDB Compass from: https://www.mongodb.com/download-center?jmp=hero#compass
2. Once downloaded, open Compass and fill in the details for the MongoDB database that you want to connect to. In our case, it is the Kaa Sandbox (130.245.144.229) MongoDB database. Usually, we just need to enter the host correctly, other details like port number etc. remain same.
3. When Compass connects to the database, find the logs for our appliacation by matching the application token for SpecSense application on the Kaa servers. The log names are available as logs_<application_token>
4. Once we are in our logs, navigate to the Schema tab.
5. In the filter field on the Schema tab, enter a filter. Ex: {'event.frequency':{$ne:0}} (This filter fetches all log records where the sampled frequency is not equal to zero. There are other operators available, which will be displayed in the autocomplete options)
6. After entering the filter, if it is syntactically correct, the 'Analyze' button next to the filter field will get activated. Click on 'Analyze'.
7. Now you will see the graphs created. You will have to unfold the 'event' tag to see the stats for power, frequency and iq. The nodenumber is another field to know how much data came from which of the nodes. This nodenumber is hardcoded in the code that runs on each of the endpoints.
