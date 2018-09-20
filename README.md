# SpectrumSensingWithNotifications

Kaa-sandbox server available at: 130.245.144.129:9080/sandbox

To start the application, first make sure the above sandbox server is running.
If not, start it on the VirtualBox installed on the Redbox.

1. Once it starts, go to the endpoint (odroid, raspberry pi or desktop)
2. If there are any changes in the source file NotificationDemo_with_actual_sampling.cpp, do the following:
  1. cd build
  2. make -j4
3. This will build the SDK and generate an executable kaa-app
4. If this is a test to calculate the data rate and if the number of samples sent is important, then drop the mongodb database before executing the executable by running drop_db.py in analysis_files directory
4. Run this executable (./kaa-app) to start collecting and sending the samples back to the server on the kaa sandbox
5. Once the test is complete, run data_rate_computation.py in analysis_files directory to get the number of samples in the database
