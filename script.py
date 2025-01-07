import subprocess
import time
import threading
import os
import sys

def read_output(process):
    while True:
        output = process.stdout.readline()
        if output == '' and process.poll() is not None:
            break
        if output:
            print(output.strip())

# Lancer nc localhost 5555
process = subprocess.Popen(['nc', 'localhost', '5555'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

# Lancer un thread pour lire les sorties du processus
thread = threading.Thread(target=read_output, args=(process,))
thread.start()

# Attendre un court instant pour s'assurer que la connexion est établie
time.sleep(1)

try:
    # Envoyer les commandes
    commands = ['JOIN #test\n', 'PASS irc\n', 'JOIN #test\n', 'PRIVMSG #test test\n']
    for command in commands:
        process.stdin.write(command)
        process.stdin.flush()
        time.sleep(0.5)  # Attendre un peu entre les commandes pour éviter les problèmes de synchronisation
except BrokenPipeError:
    print("Le processus nc s'est terminé prématurément.")

# Rediriger stdin, stdout et stderr vers le processus nc
os.dup2(process.stdin.fileno(), sys.stdin.fileno())
os.dup2(process.stdout.fileno(), sys.stdout.fileno())
os.dup2(process.stderr.fileno(), sys.stderr.fileno())

# Attendre que le thread de lecture se termine
thread.join()