from vtk import *

ren = vtkRenderer() 
iren = vtkRenderWindowInteractor() 
renwin = vtkRenderWindow() 
renwin.AddRenderer(ren) 
renwin.SetInteractor(iren)

cone = vtkConeSource()

mapper = vtkPolyDataMapper() 
mapper.SetInput(cone.GetOutput()) 
actor = vtkActor() 
actor.SetMapper(mapper) 
ren.AddActor(actor)

def keypress(obj, event):
	if obj.GetKeySym() == 's':
		print 'keypress!'

iren.AddObserver("KeyPressEvent", keypress)

iren.Initialize() 
iren.Start()

