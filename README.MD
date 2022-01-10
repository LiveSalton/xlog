```plantuml
title mmap-writer 类图
header www.salton123.com


class MmapWriter{

}

class Metadata {

}

class FileFlusher {

}

class BufferTrigger{

}

class BufferFlusher{

}


Metadata --> BufferTrigger
BufferTrigger --> MmapWriter
FileFlusher --> BufferTrigger
BufferFlusher --> FileFlusher
```

