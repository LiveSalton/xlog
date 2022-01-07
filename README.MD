```plantuml
title mmap-writer 类图
header www.salton123.com


class MmapWriter{

}

class Metadata{

}

class FileDifferential{

}

class BufferTrigger{

}

class FlushBuffer{

}


Metadata --> BufferTrigger
FlushBuffer --> BufferTrigger
BufferTrigger --> MmapWriter
FileDifferential --> BufferTrigger
FileDifferential --> MmapWriter
```

