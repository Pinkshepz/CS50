import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from datetime import datetime as dt

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get rows of transactions
    rows = db.execute("SELECT symbol, SUM(share), AVG(price),\
                      ROUND((SUM(share)*AVG(price)), 2) AS TOTAL \
                      FROM portfolio WHERE user_id = (?) GROUP BY symbol",
                      session["user_id"])

    stockNames = [lookup(stock["symbol"])["name"] for stock in rows]
    stockPrice = [usd(stock["AVG(price)"]) for stock in rows]
    stockTotal = [usd(stock["TOTAL"]) for stock in rows]

    cashRemain = db.execute("SELECT cash FROM users WHERE id=(?)",
                            session["user_id"])[0]["cash"]
    stockValue = sum([row["TOTAL"] for row in rows])

    return render_template("index.html",
                           rows=zip(rows, stockNames, stockPrice, stockTotal),
                           cash=usd(cashRemain),
                           total=usd(cashRemain+stockValue))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol and share are filled
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        if not request.form.get("shares"):
            return apology("missing share", 400)

        # Ensure symbol is valid
        if not lookup(request.form.get("symbol")):
            return apology("quote doesn't exist", 400)

        # Ensure there is not fractional, negative, and non-numeric shares
        try:
            if float(request.form.get("shares")) % 1 != 0 or float(request.form.get("shares")) < 0:
                return apology("share doesn't exist", 400)
        except:
            return apology("share doesn't exist", 400)

        # Ensure there is enough cash
        cash = db.execute("SELECT cash FROM users WHERE id = (?)",
                          session["user_id"])[0]["cash"]
        cashRequired = lookup(request.form.get("symbol"))[
            "price"] * float(request.form.get("shares"))

        if cash < cashRequired:
            return apology("not enough cash", 400)

        # INSERT transaction into the database
        db.execute("INSERT INTO portfolio (user_id, date, symbol, price, share) VALUES(?, ?, ?, ?, ?)",
                   session["user_id"],
                   dt.now().strftime("%Y-%m-%d %H:%M:%S"),
                   request.form.get("symbol"),
                   lookup(request.form.get("symbol"))["price"],
                   int(request.form.get("shares")))

        # UPDATE cash in the database
        db.execute("UPDATE users SET cash = (?)", cash-cashRequired)

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get rows of history
    rows = db.execute("SELECT symbol, share, price, date \
                      FROM portfolio \
                      WHERE user_id=(?)",
                      session["user_id"])

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Lookup quote and store in stockData
        stockData = lookup(request.form.get("symbol"))
        if not stockData:
            return apology("quote doesn't exist", 400)
        return render_template("quote.html", search=1, data=stockData, price=usd(stockData["price"]))

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html", search=0)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)

        # Ensure password and confirmed password is identical
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        # Check if username is already taken
        for name in db.execute("SELECT username FROM users"):
            if name["username"] == request.form.get("username"):
                return apology("username is already taken", 400)

        # Generate password hash
        passwordHash = generate_password_hash(request.form.get("password"))

        # INSERT new registrant's username and password into the users database
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)",
                   request.form.get("username"), passwordHash)

        # Query database for username
        userId = db.execute("SELECT id FROM users WHERE username = ?",
                            request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = userId[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol and share are filled
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        if not request.form.get("shares"):
            return apology("missing share", 400)

        # Ensure there is not fractional, negative, and non-numeric shares
        try:
            if float(request.form.get("shares")) % 1 != 0 or float(request.form.get("shares")) < 0:
                return apology("share doesn't exist", 400)
        except:
            return apology("share doesn't exist", 400)

        # Ensure there is enough shares to sell
        shareAvi = db.execute("SELECT SUM(share) FROM portfolio \
                              WHERE user_id = (?) \
                              AND symbol = (?)",
                              session["user_id"],
                              request.form.get("symbol"))[0]["SUM(share)"]
        if int(request.form.get("shares")) > shareAvi:
            return apology("not enough shares", 400)

        # INSERT transaction into the database
        db.execute("INSERT INTO portfolio (user_id, date, symbol, price, share) \
                   VALUES(?, ?, ?, ?, ?)",
                   session["user_id"],
                   dt.now().strftime("%Y-%m-%d %H:%M:%S"),
                   request.form.get("symbol"),
                   lookup(request.form.get("symbol"))["price"],
                   int(request.form.get("shares"))*(-1))

        # Calculate sell return
        cash = db.execute("SELECT cash FROM users WHERE id = (?)",
                          session["user_id"])[0]["cash"]
        cashObtained = lookup(request.form.get("symbol"))["price"] * \
            float(request.form.get("shares"))

        # UPDATE cash in the database
        db.execute("UPDATE users SET cash = (?)", cash+cashObtained)

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get list of stock names
        names = [stock["symbol"] for stock in
                 db.execute("SELECT symbol, SUM(share) FROM portfolio \
                            WHERE user_id = (?) \
                            GROUP BY symbol",
                            session["user_id"])
                 if stock["SUM(share)"] != 0]

        return render_template("sell.html", names=names)
