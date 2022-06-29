from kafka import KafkaConsumer
import json
import psycopg2
# To consume latest messages and auto-commit offsets
con = psycopg2.connect(database="yogadb", user='yogauser', password='Yogauser@123', host='3.111.186.156',
                       port='5432')
consumer = KafkaConsumer('t1',
                         group_id='test',
                         bootstrap_servers=['192.168.43.89:49155'], value_deserializer=lambda m: json.loads(m.decode('ascii')))
count = 1


cur = con.cursor()
for message in consumer:
    query = "INSERT INTO sensor(data, time) VALUES"
    i = 0
    # message value and key are raw bytes -- decode if necessary!
    # e.g., for unicode: `message.value.decode('utf-8')`
    while(i < 1000):
        x = message.value
        fsr = x["sensor"][i]["fsr"]
        time = x["sensor"][i]["time"]
        small_query = "(%s, %s)"%(fsr, time)
        print(i)
        if i==999:
            query = query + small_query
            cur.execute(query)
            con.commit()
        else:
            query = query+small_query+","
        i+=1