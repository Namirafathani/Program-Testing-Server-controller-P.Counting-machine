# Program Testing Server-Kontroler
Program untuk Testing Kontroler dan Server. Data yang dikirimkan pada server berupa data dummy hasil hitungan sensor.

a. publish data kontroler >>> Server
  topic : PSI/countingbenang/datacollector/reportdata
  payload : {"id_controller":"CTR01","id_machine":"MAC01_01","clock":"2020-02-03 16:47","count":2,"status":0,"temp_data":150,"flagsensor":1}
  
b. publish start information kontroler >>> server
  topic : PSI/countingbenang/datacollector/startcontroller
  payload : {"id_controller":"CTR01","flagstart":1}

c. publish re-start apabila push button ditekan kontroler >>> server
  topic : PSI/countingbenang/datacollector/startcontroller
  payload : {"id_controller":"CTR01","flagrestart":1}
  
d. subscribe untuk dapat mengirimkan data dummy publish ke server kontroler >>> server
  topic : PSI/countingbenang/server/replystart
  payload : {"current_time":"$2020-02-18#15:37:39/n","MAC01":95,"MAC02":100,"MAC03":0,"MAC04":0,"MAC05":0,"MAC06":0,"MAC07":0,"MAC08":0,"MAC09":0,"MAC10":0,"MAC11":0,"MAC12":0,"flagreply":1}

Setiap lima detik akan di-generate nilai dummy baru, dan apabila kontroler mendapatkan subscribe maka kontroler akan dapat melakukan publish.
