from flask import render_template, flash, redirect, url_for, session, request
from flask_wtf import form
from wtforms import ValidationError
from werkzeug.datastructures import FileStorage
from plotterapp import plotterapp, db, images
from .models import Canvas
from .forms import NewProjectForm
import base64
from .get_location import get

@plotterapp.route('/')
@plotterapp.route('/index')
def index():
	return render_template('index.html')

@plotterapp.route('/help')
def help():
	return render_template('help.html')

@plotterapp.route('/start-project', methods=['GET', 'POST'])
def project_form():
	form = NewProjectForm()
	if form.validate_on_submit():
		project = Canvas()
		project.project_name = form.project_name.data
		project.scale_type = form.scale_type.data
		project.width = form.width.data
		project.height = form.height.data
		project.mounting_distance = form.mounting_distance.data
		project.origin_distance = form.origin_distance.data
		if form.scale_type.data == '0':	#inches
			project.widthPx = form.width.data*300
			project.heightPx = form.height.data*300
			project.origin_distance = form.origin_distance.data*300
		elif form.scale_type.data == '1':		#foot
			project.widthPx = form.width.data*3600
			project.heightPx = form.height.data*3600
			project.origin_distance = form.origin_distance.data*3600
		else:		#meter
			project.widthPx = form.width.data*(283465/24)
			project.heightPx = form.height.data*(283465/24)
			project.origin_distance = form.origin_distance.data*(283465/24)
		db.session.add(project)
		db.session.commit()
		return redirect(url_for('calibration_page',id=project.id))
	return render_template('project_form.html', form=form)

@plotterapp.route('/canvas/<id>')
def canvas(id):
	project = Canvas.query.filter_by(id=id).first_or_404()
	return render_template('canvas.html',project=project)

@plotterapp.route('/render-image/<id>', methods=['GET', 'POST'])
def render_image(id):
	canvas = Canvas.query.get(id)
	data = request.get_json().get("img")
	filename = "image"+id+".png"
	canvas.image_filename = filename
	with open('plotterapp/static/images/'+filename, "wb") as fh:
		fh.write(base64.decodebytes(data.encode()))
	fh.close()
	canvas.image_url = images.url(filename)
	get(filename)
	db.session.add(canvas)
	db.session.commit()
	return "success"

@plotterapp.route('/calibration-page/<id>')
def calibration_page(id):
	project = Canvas.query.filter_by(id=id).first_or_404()
	return render_template('calibration_page.html',project=project)
	