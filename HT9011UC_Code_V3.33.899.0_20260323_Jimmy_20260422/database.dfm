object DataModule1: TDataModule1
  OldCreateOrder = True
  OnDestroy = DataModuleDestroy
  Left = 1181
  Top = 564
  Height = 479
  Width = 741
  object MotorTable: TTable
    DatabaseName = 'newhandler'
    FieldDefs = <
      item
        Name = 'Motorname'
        DataType = ftString
        Size = 23
      end
      item
        Name = 'Alias'
        DataType = ftString
        Size = 15
      end
      item
        Name = 'Direction'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'GearRatio'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'HomeDirectior'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'HomeHighSpeed'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'HomeLowSpeed'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'InitSpeed'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'JogHighSpeed'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'JogLowSpeed'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'Rate'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'SoftLimitN'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'SoftLimitP'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'Chinese'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'English'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'Port'
        DataType = ftString
        Size = 8
      end
      item
        Name = 'Enable'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'ServoAlarmOn'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'Range'
        DataType = ftString
        Size = 4
      end
      item
        Name = '1P2P'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'SensorType'
        DataType = ftString
        Size = 1
      end>
    StoreDefs = True
    TableName = 'motor.db'
    Left = 32
    Top = 88
  end
  object CylinderTable: TTable
    DatabaseName = 'newhandler'
    FieldDefs = <
      item
        Name = 'DelayIndex'
        DataType = ftString
        Size = 8
      end
      item
        Name = 'DelayCT'
        DataType = ftInteger
      end
      item
        Name = 'DelayMessage'
        DataType = ftString
        Size = 255
      end>
    StoreDefs = True
    TableName = 'cylinder.DB'
    Left = 128
    Top = 24
  end
  object SensorTable: TTable
    DatabaseName = 'newhandler'
    FieldDefs = <
      item
        Name = 'SensorName'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'InIOPort'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'InIOBit'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'InType'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'Enable'
        DataType = ftString
        Size = 1
      end>
    StoreDefs = True
    TableName = 'sensor.DB'
    Left = 128
    Top = 88
  end
  object SwitchTable: TTable
    DatabaseName = 'newhandler'
    FieldDefs = <
      item
        Name = 'SwitchName'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'OutIOPort'
        DataType = ftString
        Size = 6
      end
      item
        Name = 'OutIOBit'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'OutIOType'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'Enable'
        DataType = ftString
        Size = 1
      end>
    StoreDefs = True
    TableName = 'switch.db'
    Left = 224
    Top = 24
  end
  object SuckerTable: TTable
    DatabaseName = 'newhandler'
    TableName = 'sucker.DB'
    Left = 232
    Top = 88
  end
end
