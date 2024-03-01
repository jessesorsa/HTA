import sqlite3

# From terminal: 
# sqlite3 sessions.db
# .schema sessions
# INSERT INTO sessions (km, steps, burnt_kcal) VALUES (10, 5000, 200);
# SELECT * FROM sessions;
# etc...

# Connect to the SQLite database
con = sqlite3.connect('sessions.db')
cur = con.cursor()

# Define and execute SQL INSERT statements
sql_insert = """
    INSERT INTO sessions (session_id, km, steps, burnt_kcal)
    VALUES (?, ?, ?, ?);
"""
data = [
    (1, 10, 5000, 200),
    (2, 15, 7000, 300),
    (3, 8, 4000, 150)
]

cur.executemany(sql_insert, data)

# Commit the changes
con.commit()

# Close the connection
con.close()
