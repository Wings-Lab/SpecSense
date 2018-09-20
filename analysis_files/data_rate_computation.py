
import pymongo
import bson

import unicodedata


myclient = pymongo.MongoClient("mongodb://130.245.144.129:27017/")
mydb = myclient["kaa"]
mycol = mydb["logs_71766525981989508175"]

total_iq_size = 0
total_number_of_docs = 0
for x in mycol.find():
  iq_sample_size = len(x.get('event').get('iq').encode('utf-8'))
  iq_sample = x.get('event').get('iq').decode('utf-8')
  iq_sample = unicodedata.normalize('NFKD', iq_sample).encode('ascii','ignore')
  #print('IQ data: ',iq_sample)
  #print('IQ size:',iq_sample_size)
  total_iq_size += iq_sample_size
  total_number_of_docs += 1

print('Total number of docs: ', total_number_of_docs)
print('Total size of iq samples received (bytes): ', total_iq_size)
