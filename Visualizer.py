from vtk import *
import numpy
from PIL import Image
from StringIO import StringIO
import vtk.util.numpy_support as VN

class Visualizer:
    '''
    Encapsulates the VTK pipeline and provides accessor methods.
    '''
    def __init__(self, clientsock):
        self.client = clientsock
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
            self.renwin.Render()
            self.w2i.Modified()
            self.w2i.Update()
            imageData = self.w2i.GetOutput()

            # Convert the GL framebuffer into a numpy array
            rawImage = VN.vtk_to_numpy(imageData.GetPointData().GetScalars())
            rawImageArray = numpy.array(rawImage.flatten(), numpy.uint8)

            # convert to a PIL image so we can compress as JPG and stream over a socket
            x = self.renwin.GetSize()[0]
            y = self.renwin.GetSize()[1]
            img = Image.fromstring('RGB',(x,y), rawImageArray, 'raw', 'RGB')

            # StringIO serves as a memory endpoint for the compressed JPEG
            output = StringIO()
            img.save(output, format="JPEG")
            
            # get a byte array of the jpg then send:
            # 1. size stamp 1+8 bytes
            # 2. image buffer
            # 3. end stamp
            buf = output.getvalue()
            datasize = str(len(buf))
            datasize = datasize.zfill(8)
            print 'Sending pixel stream of size: ' + datasize + ' bytes.'
            self.client.send('s' + datasize)
            self.client.send(buf)
            self.client.send('e')

