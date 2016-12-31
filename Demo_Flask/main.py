# coding=utf-8
import os
from flask import Flask, abort, request, redirect, url_for, session, escape, send_from_directory, Response, stream_with_context
from datetime import timedelta

app = Flask(__name__)
app.secret_key = 'A0Zr98j/3yX R~XHH!jmN]LWX/,?RT'
app.permanent_session_lifetime = timedelta(minutes=3)

@app.route('/')
@app.route('/hello')
def hello_world():
    print request.args
    return 'hello world!'

@app.route('/user/<username>/<password>')
def show_user(username, password = None):
    return 'username:{0}\r\npassword:{1}'.format(username, password)

@app.route('/source/<path:file>')
def generate_large_csv(file):
    if not os.path.isfile(file):
        return abort(500)

    ext = None
    s = os.path.basename(file).split('.')
    if len(s) > 1:
        ext = s[len(s) - 1:]

    if ext in ['png', 'jpg']:
        return send_from_directory(os.path.dirname(file), os.path.basename(file), mimetype = 'image/png')

    def generate():
        with open(file) as f:
            for line in f:
                yield line
    if ext in ['h', 'cpp', 'json', 'txt']:
        return Response(stream_with_context(generate()), mimetype='text/plain')

    return send_from_directory(os.path.dirname(file), os.path.basename(file), as_attachment=True)

@app.route('/upload_file', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        file = request.files['file']
        if file:
            file.save(os.path.join('upload', file.filename))
            return redirect(url_for('show_user'), file.filename, 'hanyamin')

    return r'''
    <!doctype html>
    <title>Upload new File</title>
    <h1>Upload new File</h1>
    <form action="" method=post enctype="multipart/form-data">
      <p><input type=file name=file>
         <input type=submit value=Upload>
    </form>
    '''

@app.route('/download_file/<filename>', methods=['GET'])
def download_file(filename):
    if os.path.isfile(os.path.join('upload', filename)):
        return send_from_directory('upload', filename, as_attachment=True)
    abort(404)

@app.route('/index')
def index():
    if 'username' in session:
        return 'Logged in as %s' % escape(session['username'])
    return 'You are not logged in'

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        session['username'] = request.form['username']
        return redirect(url_for('index'))
    
    username = ""
    if 'username' in session:
        username = escape(session['username'])
    return '''
        <form action="" method="post">
            <p><input type=text name=username value="{0}">
            <p><input type=submit value=Login>
        </form>
    '''.format(username)

@app.route('/logout')
def logout():
    # 如果会话中有用户名就删除它。
    session.pop('username', None)
    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)
    