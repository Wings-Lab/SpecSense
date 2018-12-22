
import pymongo
import bson

import unicodedata


myclient = pymongo.MongoClient("mongodb://130.245.144.156:27017/")
mydb = myclient["kaa"]
mycol = mydb["logs_78266689983001043203"]
ip_prefix = "130.245.144."
total_iq_size = 0
total_number_of_docs = 0
nodes = {}
for x in mycol.find():
  iq_sample_size = len(x.get('event').get('iq').encode('utf-8'))
  iq_sample = x.get('event').get('iq').decode('utf-8')
  iq_sample = unicodedata.normalize('NFKD', iq_sample).encode('ascii','ignore')
  node = int(x.get('event').get('nodenumber'))
  ip_addr = ip_prefix + str(node)
  if ip_addr not in nodes:
    nodes[ip_addr] = 1
  else:
    nodes[ip_addr] += 1
  #print('IQ data: ',iq_sample)
  #print('IQ size:',iq_sample_size)
  total_iq_size += iq_sample_size
  total_number_of_docs += 1

"""total_records = 0
for node, num_samples in nodes.items():
  total_records += num_samples

for node, num_samples in nodes.items():
  nodes[node] = (num_samples/total_records) * 100"""
print "IP_address        ---   Number of samples"
for ip, num_samples in nodes.items():
	print ip , "  ---  " , num_samples
#print('Total number of docs: ', total_number_of_docs)
#print('Total size of iq samples received (bytes): ', total_iq_size)
#print('Nodes: ',nodes)
