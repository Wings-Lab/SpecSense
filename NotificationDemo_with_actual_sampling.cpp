#include <memory>
#include <condition_variable>

#include <kaa/Kaa.hpp>
#include <kaa/logging/Log.hpp>
#include <kaa/logging/LoggingUtils.hpp>
#include <kaa/notification/INotificationTopicListListener.hpp>
#include <kaa/common/exception/UnavailableTopicException.hpp>
#include <rtl-sdr.h>
#include "convenience/convenience.hpp"
#include <memory>
#include <string>
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <ncurses.h>
#include <kaa/log/strategies/RecordCountLogUploadStrategy.hpp>
#include <kaa/log/ILogStorageStatus.hpp>
#include <ctime>
#include <chrono>

using namespace kaa;

#define DEFAULT_SAMPLE_RATE 1024000
static constexpr auto TOPIC_LIST_UPDATE_TIMEOUT = 3;
static bool switch_flag = false;
static int samplingPeriod = 1; //seconds
static std::string numberOfBins = "256";
static std::string numberOfSpectraToAvg = "32";
static rtlsdr_dev_t *dev = NULL;
std::string cmd_for_power = "./rtl_power_fftw -n 32 -q -b 256 -f 916e6 -g 1";
static bool differentIqSampleSizes = false;
static int iq_size = 4 * 1024 * 1024;

static void set_up_device() 
{
	//static rtlsdr_dev_t *dev = NULL;
    int dev_index = 0;
	int r;
	dev_index = verbose_device_search("0");
	r = rtlsdr_open(&dev, (uint32_t)dev_index);
    if (r < 0) 
    {
        printf("Failed to open device");
		exit (1);
	}
    verbose_set_sample_rate(dev, DEFAULT_SAMPLE_RATE);
    verbose_set_frequency(dev, 915800000);
    verbose_gain_set(dev, 1);
    verbose_ppm_set(dev, 0);
    verbose_reset_buffer(dev);
}


static void toggleFlag()
{        
	int error = 9999;
	switch_flag = !switch_flag;
	if (switch_flag){
		set_up_device();
	}else{
		error = rtlsdr_close(dev);
		std::cout << "Close device status: " << error << std::endl;
	}
}

static void modifySamplingPeriod(const KaaNotification &notification)
{
	std::cout << "New sampling period :" << notification.NewSamplingFrequency << std::endl;
    int num;
    const char *c;
	c = notification.NewSamplingFrequency.c_str();
	//std::cout << "New sampling period STRING:" << c << std::endl;	
	num = std::stoi(c);
	//std::cout << "New sampling period :" << num << std::endl;
	samplingPeriod = num;
}


static void modifyNumberOfBins(const KaaNotification &notification)
{
    std::cout << "New number of bins:" << notification.NewNumberOFBins << std::endl;
    numberOfBins = notification.NewNumberOFBins.c_str();
    std::string newCommand = "./rtl_power_fftw -n "+numberOfSpectraToAvg+" -q -b "+numberOfBins+" -f 916e6 -g 1";
    std::cout << "New command: " << newCommand << std::endl;
}

static void modifyNumberOfSpectraToAvg(const KaaNotification &notification)
{
    std::cout << "New number of spectra to avg:" << notification.NewNoAvgdSpectra << std::endl;
    numberOfSpectraToAvg = notification.NewNoAvgdSpectra.c_str();
    std::string newCommand = "./rtl_power_fftw -n "+numberOfSpectraToAvg+" -q -b "+numberOfBins+" -f 916e6 -g 1";
    std::cout << "New command: " << newCommand << std::endl;
}

static void processNotification(const KaaNotification &notification, const kaa_notification::power_iq_change_choice powiqChange)
{
    const char *samplefreq;
    const char *bins;
    const char *avgdspectra;
    
    samplefreq = notification.NewSamplingFrequency.c_str();
    bins = notification.NewNumberOFBins.c_str();
    avgdspectra = notification.NewNoAvgdSpectra.c_str();
    
    //const kaa_notification::PowIq_change powiqChange = notification.PowIQ_change;
    switch(powiqChange)
    {
        case kaa_notification::Y_change:
            toggleFlag();
            std::cout << "Changing Power IQ switch" << std::endl;
            break;
        case kaa_notification::N_change:
            break;
    }
    
    if(samplefreq[0] != '0')
    {
        std::cout << "Changing Sampling Frequency" << std::endl;
        modifySamplingPeriod(notification);
    }
    if(bins[0] != '0')
    {
        std::cout << "Changing number of bins" << std::endl;
        modifyNumberOfBins(notification);
    }
    if(avgdspectra[0] != '0')
    {
        std::cout << "Changing number of spectra to average" << std::endl;
        modifyNumberOfSpectraToAvg(notification);
    }
}

#define READ 0
#define WRITE 1

#define RAW_SIZE (4 * 1024) 
static	int exec_raw(unsigned char *array) 
{
    uint32_t out_block_size = RAW_SIZE;
    int n_read = RAW_SIZE;
     std::cout << "Device info:" << dev << std::endl; 
    int r = rtlsdr_read_sync(dev, array, out_block_size, &n_read);
    if (r < 0) 
    {
		printf("Error in reading");
        return (1);
    }
	return (0);
}

static void extractData(std::shared_ptr<IKaaClient> kaaClient)
{
    FILE *fp;
    int sampleCount = 0;
    if (!switch_flag) 
    {
		char buffer[128];
		const char *cmd = cmd_for_power.c_str();
		//std::shared_ptr<FILE> pipe(popen(cmd, "r"),pclose);
		fp  = popen(cmd, "r");

		if (!pipe) throw std::runtime_error("popen() failed!");
                sampleCount = 0;
			while (!feof(fp)) 
            {
				if (fgets(buffer, 128, fp) != nullptr) 
                {
					std::stringstream ss;
					ss << buffer;
					kaa::KaaUserLogRecord logRecord;
					ss >> logRecord.frequency;
					ss >> logRecord.power;
					logRecord.nodenumber = 110;
					kaaClient->addLogRecord(logRecord);
					std::cout << "Sampled power and frequency: " << logRecord.power << " " << logRecord.frequency << std::endl;
                                        sampleCount++;
			    }
                    if(sampleCount == 3)
                    	break;
		    }
		pclose(fp);
        /*kaa::KaaUserLogRecord logRecord;
	//srand(200);
        logRecord.frequency = (rand() % 20) + 8;
	logRecord.power = (rand() % 20) + 8;
        kaaClient->addLogRecord(logRecord);*/
	}
    else 
    {
	    unsigned char *array = NULL;
            array = (unsigned char *)malloc(RAW_SIZE);
            if (array == NULL)
            	std::cout<<"Memory problem at the array allocation\n";
            
	    exec_raw(array);	
	    std::vector<unsigned char> vec(array, array + RAW_SIZE);
	    kaa::KaaUserLogRecord logRecord;
	    logRecord.frequency = logRecord.power = 0;
	    logRecord.iq = vec;
	    std::cout << "Size of iq sample: " << vec.size() << std::endl;
	    //std::cout << "Size of logrecord: " << logRecord.size() << std::endl;
	    std::cout << "Size of logrecord.iq: " << logRecord.iq.size() << std::endl;

	    logRecord.nodenumber = 2;
	    kaaClient->addLogRecord(logRecord);
	    std::cout << "Size of logrecord.iq after sendng: " << logRecord.iq.size() << std::endl;
	    if (logRecord.iq.size() != iq_size ){
	    	differentIqSampleSizes = true;
	    }
	    std::cout << "IQ data entered: " << std::endl;
   	    free(array);
        /*kaa::KaaUserLogRecord logRecord;
	//srand(50);
        logRecord.frequency = (rand() % 20) + 8;
		logRecord.power = (rand()% 20) + 8;
        kaaClient->addLogRecord(logRecord);*/
    }
}

class NotificationListener : public INotificationListener 
{
public:
    NotificationListener(std::shared_ptr<IKaaClient> kaaClient):
        kaaClient(kaaClient)
    {}

    void onNotification(const int64_t topicId, const KaaNotification &notification) override
    {
        auto topics = kaaClient->getTopics();
        const auto &topic = std::find_if(topics.begin(), topics.end(),
                [topicId](const Topic &t) { return t.id == topicId; });
        std::cout << (boost::format("Notification for topic id '%1%' and name '%2%' received") % topicId % topic->name) << std::endl;
        processNotification(notification, notification.power_iq_change);
	//std::cout << "Calling toggle flag method...." << std::endl;
	//processNotification(notification);
	
    }

private:
    std::shared_ptr<IKaaClient> kaaClient;
};


int main()
{
    clock_t start = clock();
    auto tstart = std::chrono::steady_clock::now();
    auto kaaClient = Kaa::newClient();
    int counter = 0;
   
   //set_up_device();
    kaaClient->setLogUploadStrategy(std::make_shared<kaa::RecordCountLogUploadStrategy>(1, kaaClient->getKaaClientContext()));
            
    NotificationListener notificationListener(kaaClient);
    kaaClient->addNotificationListener(notificationListener);
    std::cin.clear();
    kaaClient->start();

    std::cout << "Notification demo started" << std::endl;

    const auto &topics = kaaClient->getTopics();

    for(const auto &topic : topics)
    {
    	std::cout << (boost::format("Topic ID-> %1%, Topic name-> %2%") % topic.id % topic.name) << std::endl;
    }
    switch_flag = false; //Start with taking power-freq samples

    //if (switch_flag)
	//set_up_device();
    while (true) 
    {
	    /*if(counter == 1){
		std::cout << "Breaking out" << std::endl;
		break;
	    }*/
	    counter++;
            extractData(kaaClient);
	    std::cout << counter << "--->In loop with flag : " << switch_flag << std::endl;
	    std::this_thread::sleep_for (std::chrono::seconds(samplingPeriod));
	    //if(counter == 100)
		//break;
    }

    clock_t stopTime = clock();
    auto tend = std::chrono::steady_clock::now();

    std::cout << "Execution time: " << std::chrono::duration_cast<std::chrono::seconds>(tend - tstart).count() << std::endl;
    if(differentIqSampleSizes == true){
    	std::cout << "Different sizes for iq samples found" << std::endl;
    }
    kaaClient->stop();

    return EXIT_SUCCESS;
}
