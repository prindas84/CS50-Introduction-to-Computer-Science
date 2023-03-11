import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from werkzeug.utils import secure_filename
from tempfile import mkdtemp

from helpers import login_required


app = Flask(__name__)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///admin.db")

# Configure the file path for saving images
app.config["IMAGE_UPLOADS"] = "/home/ubuntu/final_project/static/images/models"
app.config["ALLOWED_FILE_EXTENSIONS"] = ["PNG", "JPG", "JPEG"]


# Checks the file name for the allowable extensions
def allowed_image(filename):

    # We only want files with a . in the filename
    if not "." in filename:
        return False

    # Split the extension from the filename
    ext = filename.rsplit(".", 1)[1]

    # Check if the extension is in ALLOWED_FILE_EXTENSIONS
    if ext.upper() in app.config["ALLOWED_FILE_EXTENSIONS"]:
        return True
    else:
        return False


# Route for the index page
@app.route("/")
def index():
    return render_template("index.html")
    

# Route for the about us page
@app.route("/about")
def about():
    return render_template("about-us.html")
    

# Route for the add model function - LOGIN REQUIRED
@app.route("/add_model", methods=["GET", "POST"])
@login_required
def add_model():

    # If the request methos is POST
    if request.method == "POST":

        # Check the user entered a name for the model
        if not request.form.get("model_name"):
            error = "Please Enter A Model Name"
            return render_template("add_model.html", error=error)

        # Check the user entered a phone number for the model
        if not request.form.get("model_phone"):
            error = "Please Enter A Phone Number"
            return render_template("add_model.html", error=error)

        # Check the user entered an email address for the model
        if not request.form.get("model_email"):
            error = "Please Enter An Email Address"
            return render_template("add_model.html", error=error)

        # Check the user uploaded an image for the model
        if not request.files["model_image"]:
            error = "Please Upload An Image"
            return render_template("add_model.html", error=error)

        # If all required fields submitted
        else:

            image = request.files["model_image"]

            # Check the image file has a filename
            if image.filename == "":
                error = "Incorrct Filname"
                return render_template("add_model.html", error=error)

            # Check the file has an acceptable file extension, as listed above in the configuration
            if not allowed_image(image.filename):
                error = "Incorrct File Extension"
                return render_template("add_model.html", error=error)

            # If image file is acceptable
            else:
                
                # Give the file a secure filename
                filename = secure_filename(image.filename)

                # Save the file to the server
                image.save(os.path.join(app.config["IMAGE_UPLOADS"], filename))

                # Save the file path to the database for use later in HTML
                path = "/static/images/models/" + filename

                # Load all model details into the database
                db.execute("INSERT INTO model (name, phone, email, image) VALUES(?, ?, ?, ?)",
                           request.form.get("model_name"), request.form.get("model_phone"),
                           request.form.get("model_email"), path)

                # Redirect the user to the profiles page after the function has completed.
                return redirect("/profiles")

    # If the request method is GET 
    else:
        return render_template("add_model.html")

# Route for tha admin dashboard page - LOGIN REQUIRED
@app.route("/admin")
@login_required
def admin():
    return render_template("admin.html")

# Route for the contact us page
@app.route("/contact")
def contact():
    return render_template("contact-us.html")

# Route for the enquiries page - LOGIN REQUIRED
@app.route("/enquiries", methods=["GET", "POST"])
@login_required
def enquiries():

    # If the request method is POST
    if request.method == "POST":
        
        # Get the value from the delete button
        value = request.form.get("delete")

        # Delete the record from the enquiry database
        db.execute("DELETE FROM enquiry WHERE id = ?", value)
        return redirect("/enquiries")

    # If the request methos is GET
    else:
        
        # Query the database for the enquiries and pass them to the HTML template.
        enquiry_list = db.execute("SELECT * FROM enquiry")
        return render_template("enquiries.html", enquiry_list=enquiry_list)


# Route for the login page
@app.route("/login", methods=["GET", "POST"])
def login():

    # Forget any user_id
    session.clear()

    # If the request method is POST
    if request.method == "POST":

        # Check the user has entered a username
        if not request.form.get("username"):
            error = "Please Enter Your Username"
            return render_template("login.html", error=error)

        #Check the user has entered a password
        if not request.form.get("password"):
            error = "Please Enter Your Password"
            return render_template("login.html", error=error)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            error = "Invalid Username / Password"
            return render_template("login.html", error=error)

        # If the user as submitted a correct username and password
        else:
            
            # Remember which user has logged in
            session["user_id"] = rows[0]["id"]

            # Redirect user to home page and post a success message to the screen
            flash("You have successfully logged in!")
            return redirect("/admin")

    # If the request method is GET
    else:
        return render_template("login.html")
        

# Route for the logout page
@app.route("/logout")
def logout():

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


# Route for the models page
@app.route("/models")
def models():

    # Get a list of all of the models in the system
    models_list = db.execute("SELECT * FROM model")

    # Declare some variables required to style 4 x 4 grid layout correctly.
    row_count = int(len(models_list) / 4)
    extra_count = len(models_list) % 4
    last_position = row_count * 4
    blank_position = 4 - extra_count 

    # Pass the list and the variables to the HTML template
    return render_template("our-models.html", models_list=models_list, row_count=row_count, extra_count=extra_count, last_position=last_position, blank_position=blank_position)

# Route for the profiles page - LOGIN REQUIRED
@app.route("/profiles", methods=["GET", "POST"])
@login_required
def profiles():

    # If the request method is POST
    if request.method == "POST":
        
        # Find the value of the delete button
        value = request.form.get("delete")

        # Delete the record from the database
        db.execute("DELETE FROM model WHERE id = ?", value)
        return redirect("/profiles")

    # If the request method is GET
    else:
        
        # Find the list of models in the system to display and pass them to the HTML template
        models_list = db.execute("SELECT * FROM model")
        
        return render_template("profiles.html", models_list=models_list)
        
    
# Route for submitting the enquiry form on the contact us page
@app.route("/submit", methods=["POST"])
def submit():

    # Check the user has entered an enquiry name
    if not request.form.get("enquiry_name"):
        error = "Please Provide Your Name"
        return render_template("contact-us.html", error=error, scrollToAnchor="error_message")

    # Check the user has entered an enquiry phone number
    if not request.form.get("enquiry_phone"):
        error = "Please Provide Your Phone Number"
        return render_template("contact-us.html", error=error, scrollToAnchor="error_message")

    # Check the user has entered an enquiry email address
    if not request.form.get("enquiry_email"):
        error = "Please Provide Your Email Address"
        return render_template("contact-us.html", error=error, scrollToAnchor="error_message")

    # Check the user has entered an enquiry email address
    if not request.form.get("enquiry_message"):
        error = "Please Include A Message"
        return render_template("contact-us.html", error=error, scrollToAnchor="error_message")

    # If the user has entered all required data
    else:
        
        # Enter the enquiry data into the database
        db.execute("INSERT INTO enquiry (name, company, phone, email, message) VALUES (?, ?, ?, ?, ?)",
                   request.form.get("enquiry_name"), request.form.get("enquiry_company"), request.form.get("enquiry_phone"),
                   request.form.get("enquiry_email"), request.form.get("enquiry_message"))

        # Post a success message to the screen
        flash("Thank you for your enquiry. We will contact you shortly!")
        return render_template("contact-us.html")

