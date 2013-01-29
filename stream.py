from vtk import *
import numpy
from PIL import Image
from StringIO import StringIO
import vtk.util.numpy_support as VN

ren = vtkRenderer() 
iren = vtkRenderWindowInteractor() 
renwin = vtkRenderWindow() 
renwin.AddRenderer(ren) 
renwin.SetInteractor(iren)

# create a jpg writer to write pixel data to jpg in memory
w2i = vtk.vtkWindowToImageFilter()
w2i.SetInput(renwin)

cone = vtkConeSource()

mapper = vtkPolyDataMapper() 
mapper.SetInput(cone.GetOutput()) 
actor = vtkActor() 
actor.SetMapper(mapper) 
ren.AddActor(actor)

def keypress(obj, event):
	if obj.GetKeySym() == 's':
		print 'Sending pixel stream.'
        w2i.Update()
        renwin.Render()
        imageData = w2i.GetOutput()
        raw = VN.vtk_to_numpy(imageData.GetPointData().GetScalars())
        array = numpy.array(raw.flatten(), numpy.uint8)
        img = Image.fromstring('RGB',(300,300), array, 'raw', 'RGB')
        img.save('my.png')

iren.AddObserver("KeyPressEvent", keypress)

iren.Initialize() 
iren.Start()

