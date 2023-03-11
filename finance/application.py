import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    # Get the user details to find how much money they have in their account
    value = 0
    user = db.execute("SELECT * FROM users WHERE id = ?", session.get("user_id"))
    funds = user[0]["cash"]

    # Generate a list of all of the stocks the user is holding, then find the value of all of the stocks the user owns
    holding = db.execute("SELECT symbol, name, shares FROM holding WHERE user_id = ? GROUP BY symbol", session.get("user_id"))

    for row in holding:
        update = lookup(row["symbol"])
        row["price"] = update["price"]
        row["total"] = row["shares"] * update["price"]
        value += row["total"]

    balance = value + funds

    # Render the index page passing through the required data for the table
    return render_template("index.html", holding=holding, funds=funds, balance=balance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    # If the user is submitting data to the route
    if request.method == "POST":

        amount = request.form.get("shares")

        # Ensure a symbol was submitted
        if not request.form.get("symbol"):
            return apology("enter a valid symbol", 400)

        # Ensure the number of shares to purchase was submitted
        if not request.form.get("shares"):
            return apology("invalid share amount", 400)

        # REFERENCE: https://www.reddit.com/r/cs50/comments/gevxqi/finance_again_buy_is_rejecting_fractional_shares/
        if not amount.isdigit():
            return apology("invalid share amount", 400)

        # Ensure the number of shares to purchase was greater than 0
        if amount.isdigit():
            if int(amount) < 1:
                return apology("invalid share amount", 400)
            
        # If the stock symbol is invalid, return a 400 page
        if (lookup(request.form.get("symbol")) == None):
            return apology("invalid stock symbol", 400)

        # If the stock symbol is valid, make the purchase
        else:
            stock = lookup(request.form.get("symbol"))
            user = db.execute("SELECT * FROM users WHERE id = ?", session.get("user_id"))
            funds = user[0]["cash"]
            purchase_price = stock["price"] * int(amount)
            balance = funds - purchase_price

            # Check the user has sufficient funds to pay for the purchase
            if (purchase_price > funds):
                return apology("insufficient funds", 400)

            # If the user has sufficient funds, record the purchase and redirect to the homepage
            else:
                check = db.execute("SELECT * FROM holding WHERE symbol = ? AND user_id = ?", stock["symbol"], user[0]["id"])

                # If the user does not own this stock already, add the purchase to the database
                if not check:
                    db.execute("INSERT INTO holding (user_id, symbol, name, shares) VALUES (?, ?, ?, ?)",
                               user[0]["id"], stock["symbol"], stock["name"], amount)
                    db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, user[0]["id"])
                    db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                               user[0]["id"], stock["symbol"], amount, stock["price"])
                    flash("Bought!")
                    return redirect("/")
                
                # If the user already owns this stock, update the current holdings
                else:
                    new_shares = int(check[0]["shares"]) + int(amount)
                    db.execute("UPDATE holding SET shares = ? WHERE symbol = ? AND user_id = ?",
                               new_shares, stock["symbol"], user[0]["id"])
                    db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, user[0]["id"])
                    db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                               user[0]["id"], stock["symbol"], amount, stock["price"])
                    flash("Bought!")
                    return redirect("/")

    else:
        return render_template("buy.html")
        

@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    
    # If the user is submitting data to the route
    if request.method == "POST":
        
        amount = request.form.get("deposit")
        
        # Ensure deposit amount was submitted
        if not request.form.get("deposit"):
            return apology("invalid deposit amount", 400)
            
        # Ensure deposit amount was more than 0
        if int(amount) < 1:
            return apology("invalid deposit amount", 400)
            
        # Update the users balance
        user = db.execute("SELECT * FROM users WHERE id = ?", session.get("user_id"))
        balance = float(user[0]["cash"]) + float(amount)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, session.get("user_id"))

        return redirect("/")
        
    else:
        return render_template("deposit.html")


@app.route("/history")
@login_required
def history():
    
    # Query the history table and pass it to the history html file.
    history = db.execute("SELECT * FROM history WHERE user_id = ?", session.get("user_id"))
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must enter stock symbol", 400)

        # If the sotck symbol is invalid, return a 400 page
        if (lookup(request.form.get("symbol")) == None):
            return apology("invalid stock symbol", 400)

        # If the stock symbol is valid, return the quote
        else:
            stock = lookup(request.form.get("symbol"))
            name = stock["name"]
            symbol = stock["symbol"]
            price = usd(stock["price"])
            return render_template("quoted.html", name=name, symbol=symbol, price=price)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    # Forget any user_id
    session.clear()

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password and the password confirmation match
        elif (request.form.get("password") != request.form.get("confirmation")):
            return apology("password doesn't match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username does not exists
        if len(rows) == 1:
            return apology("username is taken", 400)

        # Register the user
        else:
            password = request.form.get("password")
            hashed = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get("username"), hashed)

            # Query database for username
            rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

            # Ensure username exists and password is correct
            if len(rows) != 1:
                return apology("system error", 400)

            # Remember which user has logged in
            session["user_id"] = rows[0]["id"]

            # Redirect user to home page
            return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must select a stock", 400)

        if not request.form.get("shares"):
            return apology("invalid share amount", 400)
       
        user = session.get("user_id")
        stock = lookup(request.form.get("symbol"))
        amount = request.form.get("shares")
        history_amount = 0 - int(amount)
        count = db.execute("SELECT shares FROM holding WHERE symbol = ?", request.form.get("symbol"))
        funds = db.execute("SELECT cash FROM users WHERE id = ?", user)
        
        if int(amount) < 1:
            return apology("invalid share amount", 400)

        if int(amount) > int(count[0]["shares"]):
            return apology("invalid share amount", 400)
            
        else:
            new_count = int(count[0]["shares"]) - int(amount)
            balance = funds[0]["cash"] + (int(amount) * stock["price"])
            
            if int(amount) == int(count[0]["shares"]):
                db.execute("DELETE FROM holding WHERE symbol = ? AND user_id = ?", stock["symbol"], user)
                db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, user)
                db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                           user, stock["symbol"], history_amount, stock["price"])
                flash("Sold!")
                return redirect("/")
            
            else:
                db.execute("UPDATE holding SET shares = ? WHERE symbol = ? AND user_id = ?", new_count, stock["symbol"], user)
                db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, user)
                db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                           user, stock["symbol"], history_amount, stock["price"])
                flash("Sold!")
                return redirect("/")
            
    else:

        # Generate a list of all of the stocks the user is holding, then find the value of all of the stocks the user owns
        holding = db.execute("SELECT symbol, name, shares FROM holding WHERE user_id = ? GROUP BY symbol", session.get("user_id"))

        return render_template("sell.html", holding=holding)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
