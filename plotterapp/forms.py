from flask_wtf import FlaskForm
from wtforms import StringField,FloatField, SelectField, BooleanField, SubmitField
from wtforms.validators import DataRequired

class NewProjectForm(FlaskForm):
	project_name = StringField('Project Name', validators=[DataRequired()])
	SCALES = [
		('in', 'inch'),
		('ft', 'feet'),
		('m', 'meter'),
	]
	scale_type = SelectField('Select scale', choices=SCALES, validators=[DataRequired()])
	width = FloatField('Width', validators=[DataRequired()])
	height = FloatField('Height', validators=[DataRequired()])
	mounting_distance = FloatField('Distance between mounting points', validators=[DataRequired()])
	origin_distance = FloatField('Vertical distance to origin')
	submit = SubmitField('Create')