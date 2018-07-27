from flask import render_template, flash, redirect, url_for
from plotterapp import plotterapp

@plotterapp.route('/')
@plotterapp.route('/index')
def index():
	return render_template('index.html')