import pymongo
import bson

myclient = pymongo.MongoClient("mongodb://130.245.144.129:27017/")
mydb = myclient["kaa"]
mydb.drop_collection("logs_71766525981989508175")


