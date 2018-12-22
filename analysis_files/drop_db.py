import pymongo
import bson

myclient = pymongo.MongoClient("mongodb://130.245.144.156:27017/")
mydb = myclient["kaa"]
mydb.drop_collection("logs_78266689983001043203")
print "database dropped"

