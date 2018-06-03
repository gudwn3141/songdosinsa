from flask import Flask, session, redirect, url_for, request, render_template
import pymysql

app = Flask(__name__)
@app.route('/back')
def back() :
    return render_template('back.html')

@app.route('/user')
def showUserName():
    return render_template('user.html',
                           myteam = session['myteam'],
                           name = session['userName'],
                           gender = session['gender'],
                           age = session['age'],
                           competitor = session['competitor']
                           )


@app.route('/')
def Mode() :
    return render_template('Mode.html')

@app.route('/resister', methods=['POST' ,'GET'])
def resister():
    if request.method =='POST':
        if request.form["Mode"]=="one":
            return render_template('login_one.html')
        else:
            return render_template('login_two.html')

@app.route('/loginone', methods=['POST','GET'])
def loginone():
    if request.form['play'] == 'back':
        return redirect(url_for('Mode'))
    else:

        if request.method == 'POST':
            session['userName'] = request.form['userName']
            session['gender'] = request.form['gender']
            session['age'] = request.form['age']
            session['myteam'] = request.form['myteam']
            session['competitor'] = request.form['competitor']

            try:
                conn = pymysql.connect(host='raspberrydb.cvlmaax7vr80.ap-northeast-2.rds.amazonaws.com',
                                       user='raspberrypi',
                                       password='raspberrypi',
                                       db='raspberrypi',
                                       charset='utf8mb4'
                                       )
                curs = conn.cursor()
                sql0 = "SELECT id FROM users WHERE nickname=%s"
                curs.execute(sql0,request.form['competitor'])
                data = curs.fetchall()

                for row in data:
                    data = row[0]
                if data:
                    sql="INSERT INTO users(nickname,age,gender,myteam,yourteam,you_nickname) VALUE(%s,%s,%s,%s,%s,%s)"
                    curs.execute(sql, (
                                       session['userName'],
                                       session['age'],
                                       session['gender'],
                                       session['myteam'],
                                       'yourteam',
                                       session['competitor']
                                      )
                                 )
                    ##sql2 = "ALTER TABLE ADD speedtest abcde VARCHAR(100)"
                    sql2 = "ALTER TABLE speedtest ADD {} VARCHAR(100)".format(session['userName'])
                    curs.execute(sql2)

                    conn.commit()
                else:

                      return redirect(url_for('back'))
                conn.close()
            except:
                return redirect(url_for('back'))
            return redirect(url_for('showUserName'))
        else:
            return 'login failed'

@app.route('/logintwo', methods=['POST','GET'])
def logintwo():
    if request.form['play'] == 'back':
        return redirect(url_for('Mode'))
    else:

        if request.method == 'POST':
            session['userName'] = request.form['userName']
            session['gender'] = request.form['gender']
            session['age'] = request.form['age']
            session['myteam'] = request.form['myteam']
            session['competitor'] = request.form['competitor']

            try:
                conn = pymysql.connect(host='raspberrydb.cvlmaax7vr80.ap-northeast-2.rds.amazonaws.com',
                                       user='raspberrypi',
                                       password='raspberrypi',
                                       db='raspberrypi',
                                       charset='utf8mb4'
                                       )
                curs = conn.cursor()
                sql0 = "SELECT id FROM users WHERE myteam=%s"
                curs.execute(sql0,request.form['competitor'])
                data = curs.fetchall()

                for row in data:
                    data = row[0]
                if data:
                    sql="INSERT INTO users(nickname,age,gender,myteam,yourteam,you_nickname) VALUE(%s,%s,%s,%s,%s,%s)"
                    curs.execute(sql, (
                                       session['userName'],
                                       session['age'],
                                       session['gender'],
                                       session['myteam'],
                                       session['competitor'],
                                       'yournickname'
                                      )
                                 )
                    ##sql2 = "ALTER TABLE ADD speedtest abcde VARCHAR(100)"
                    sql2 = "ALTER TABLE speedtest ADD {} VARCHAR(100)".format(session['userName'])
                    curs.execute(sql2)

                    conn.commit()
                else:

                      return redirect(url_for('back'))
                conn.close()
            except:
                return redirect(url_for('back'))
            return redirect(url_for('showUserName'))
        else:
            return 'login failed'

@app.route('/start')
def start():

    return render_template('start.html', name = session['userName'])

app.secret_key = 'abcdefgadsjflkjsdljjdlsjfkja'

if __name__ == "__main__":
    app.debug = True
    app.run()

## host='0.0.0.0', port=5002, debug=True
