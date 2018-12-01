import unittest
import time
import subprocess, signal
import os

class TestServerMethods(unittest.TestCase):

	def test_send_msg(self):
		startServerCommand = 'python ../Server/server.py &'
		sendMsgCommand = 'python send_to_server_test.py'
		serverProcess = subprocess.Popen('exec ' + startServerCommand, shell=True, stdout=subprocess.PIPE)
		time.sleep(1)
		clientProcess = subprocess.check_call(sendMsgCommand, shell=True)
		os.kill(serverProcess.pid+1, signal.SIGKILL) # python/linux different process counting
		self.assertEqual(clientProcess, 0)

if __name__ == '__main__':
	unittest.main()
