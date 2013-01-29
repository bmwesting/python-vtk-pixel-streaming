from vtk import *
import numpy
from PIL import Image
from StringIO import StringIO
import vtk.util.numpy_support as VN

class Visualizer:
    '''
    Encapsulates the VTK pipeline and provides accessor methods.
    '''
    def __init__(self):
        self.ren = vtkRenderer() 
        self.iren = vtkRenderWindowInteractor() 
        self.renwin = vtkRenderWindow() 
        self.renwin.AddRenderer(self.ren) 
        self.renwin.SetInteractor(self.iren)
        # The WindowToImageFilter grabs the framebuffer pixels
        self.w2i = vtk.vtkWindowToImageFilter()
        self.w2i.SetInput(self.renwin)
        self.cone = vtkConeSource()
        self.mapper = vtkPolyDataMapper() 
        self.mapper.SetInput(self.cone.GetOutput()) 
        self.actor = vtkActor() 
        self.actor.SetMapper(self.mapper) 
        self.ren.AddActor(self.actor)
        self.iren.AddObserver("KeyPressEvent", self.keyPressHandler)

    def start(self):
        self.iren.Initialize()
        self.iren.Start()

    def keyPressHandler(self, obj, event):
        '''
        Grabs the current framebuffer, coverts it to a JPEG, and sends it through the socket.
        '''
        if obj.GetKeySym() == 's':
            print 'Sending pixel stream.'
            self.renwin.Render()
            self.w2i.Update()
            imageData = self.w2i.GetOutput()

            # Convert the vtk 2d framebuffer into a numpy array
            rawImage = VN.vtk_to_numpy(imageData.GetPointData().GetScalars())
            rawImageArray = numpy.array(rawImage.flatten(), numpy.uint8)

            # convert to a PIL image so we can compress as JPG and stream over a socket
            img = Image.fromstring('RGB',(300,300), rawImageArray, 'raw', 'RGB')

            # StringIO servers as a memory endpoint for the compressed JPEG
            output = StringIO()
            img.save(output, format="JPEG")
