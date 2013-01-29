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

# The WindowToImageFilter grabs the framebuffer pixels
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
        renwin.Render()
        w2i.Update()
        imageData = w2i.GetOutput()

        # Convert the vtk 2d framebuffer into a numpy array
        rawImage = VN.vtk_to_numpy(imageData.GetPointData().GetScalars())
        rawImageArray = numpy.array(rawImage.flatten(), numpy.uint8)

        # convert to a PIL image so we can compress as JPG and stream over a socket
        img = Image.fromstring('RGB',(300,300), rawImageArray, 'raw', 'RGB')

        # StringIO servers as a memory endpoint for the compressed JPEG
        output = StringIO()
        img.save(output, format="JPEG")
        #img.show()


iren.AddObserver("KeyPressEvent", keypress)

iren.Initialize() 
iren.Start()

