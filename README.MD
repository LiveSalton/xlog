```plantuml
title mmap-writer 类图
header www.salton123.com


class MmapWriter{

}

class Metadata{

}

class FileDifferential{

}

class FileBuffer{

}

class FlushBuffer{

}


Metadata --> FileBuffer
FlushBuffer --> FileBuffer
FileBuffer --> MmapWriter
FileDifferential --> FileBuffer
FileDifferential --> MmapWriter
```

