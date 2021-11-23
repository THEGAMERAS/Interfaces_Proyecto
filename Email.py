import smtplib
import sys

server = smtplib.SMTP('smtp.gmail.com',587)
server.starttls()
server.login("valpabbb","mimipab1")

#parser = int(input(""))
parser = int(sys.argv[1])
print(parser)

def SendMsg():
	if parser == 1:
		msg = "Bath"
		server.sendmail("Bath","valpabbb@gmail.com",msg)

	elif parser == 2:
		msg = "Food"
		server.sendmail("Food","valpabbb@gmail.com",msg)

	elif parser == 3:
		msg = "Wipe"
		server.sendmail("Wipe", "valpabbb@gmail.com",msg)

	else:
		print("Invalid command")

SendMsg()
server.quit()
