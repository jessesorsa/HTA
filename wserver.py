from flask import Flask
from flask_cors import CORS
from flask import render_template
from flask import jsonify
from flask import Response

import db
import hike

app = Flask(__name__)
CORS(app)
hdb = db.HubDatabase()


@app.route('/')  # main
def get_home():
    sessions = hdb.get_sessions()
    return jsonify(sessions)


@app.route('/sessions')
def get_sessions():
    sessions = hdb.get_sessions()
    return jsonify(sessions)


@app.route('/sessions/<id>')
def get_session_by_id(id):
    session = hdb.get_session(id)
    return jsonify(hike.to_list(session))


@app.route('/sessions/<id>/delete')
def delete_session(id):
    hdb.delete(id)
    print(f'DELETED SESSION WITH ID: {id}')
    return jsonify({'message': f'Session with ID {id} has been deleted'})


if __name__ == "__main__":
    app.run('0.0.0.0', debug=True)
