import threading
import pymongo
import bson

myclient = pymongo.MongoClient("mongodb://130.245.144.156:27017/")
mydb = myclient["kaa"]

def drop_db():
	threading.Timer(120.0, drop_db).start()
	mydb.drop_collection("logs_78266689983001043203")
	print("Dropped DB")

drop_db()