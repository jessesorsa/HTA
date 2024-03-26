import sqlite3

import hike
import threading

DB_FILE_NAME = 'sessions.db'

DB_SESSION_TABLE = {
    "name": "sessions",
    "cols": [
        "session_id integer PRIMARY KEY",
        "km integer",
        "steps integer",
        "burnt_kcal integer",
        "start_time text",
        "duration text"
    ]
}

# lock object so multithreaded use of the same
# HubDatabase object


class HubDatabase:
    """Hiking sesssion database interface class.

    An object of this class enables easy retreival and management of the
    hiking database content. If the database does not exist, the instantiation
    of this class will create the database inside `DB_FILE_NAME` file.

    Arguments:
        lock: lock object so multithreaded use of the same HubDatabase object
              is safe. sqlite3 does not allow the same cursor object to be
              used concurrently.
        con: sqlite3 connection object
        cur: sqlite3 cursor object
    """

    lock = threading.Lock()

    def __init__(self):
        self.con = sqlite3.connect(DB_FILE_NAME, check_same_thread=False)
        self.cur = self.con.cursor()

        table_name = DB_SESSION_TABLE["name"]
        columns = DB_SESSION_TABLE["cols"]
        column_definitions = ', '.join(columns)
        create_table_sql = f"CREATE TABLE IF NOT EXISTS {table_name} ({column_definitions})"
        self.cur.execute(create_table_sql)

        self.con.commit()

    def save(self, s: hike.HikeSession):   
        try:
            self.lock.acquire()

            try:
                self.cur.execute(
                    f"INSERT INTO {DB_SESSION_TABLE['name']} VALUES ({s.id}, {s.km}, {s.steps}, {s.kcal}, '{s.start_time}', '{s.duration}')")
            except sqlite3.IntegrityError:
                print(
                    "WARNING: Session ID already exists in database! Aborting saving current session.")

            self.con.commit()
        finally:
            self.lock.release()

    def delete(self, session_id: int):
        try:
            self.lock.acquire()

            self.cur.execute(
                f"DELETE FROM {DB_SESSION_TABLE['name']} WHERE session_id = {session_id}")
            self.con.commit()
        finally:
            self.lock.release()

    def get_sessions(self) -> list[hike.HikeSession]:
        try:
            self.lock.acquire()
            rows = self.cur.execute(
                f"SELECT * FROM {DB_SESSION_TABLE['name']}").fetchall()
        finally:
            self.lock.release()
        print(rows)
        return rows

    def get_session(self, session_id: int) -> hike.HikeSession:
        try:
            self.lock.acquire()
            rows = self.cur.execute(
                f"SELECT * FROM {DB_SESSION_TABLE['name']} WHERE session_id = {session_id}").fetchall()
        finally:
            self.lock.release()

        return hike.from_list(rows[0])

    def __del__(self):
        self.cur.close()
        self.con.close()
