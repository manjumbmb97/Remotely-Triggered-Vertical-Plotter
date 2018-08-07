from flask import render_template, flash, redirect, url_for, session
from flask_wtf import form
from plotterapp import plotterapp, db
from .models import Canvas
from .forms import NewProjectForm

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
		db.session.add(project)
		db.session.commit()
		return redirect(url_for('canvas',id=project.id))
	return render_template('project_form.html', form=form)

@plotterapp.route('/canvas/<id>')
def canvas(id):
	project = Canvas.query.filter_by(id=id).first_or_404()
	return render_template('canvas.html',project=project)
