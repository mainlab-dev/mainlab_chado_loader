<?php
/**
 * The declaration of MCL_TABLE class.
 *
 */
class MCL_TABLE {

  /**
   * Class data mebers.
   */
  private $name         = NULL;
  private $schema_name  = NULL;
  private $class_name   = NULL;
  private $attrs        = NULL;
  private $primary_key  = NULL;
  private $unique_keys  = NULL;
  private $sqls         = NULL;
  private $data_types   = NULL;
  private $constraints  = NULL;

  /**
   * Implements the class constructor.
   */
  public function __construct($name, $schema_name, $class_name) {
    $this->name         = $name;
    $this->schema_name  = $schema_name;
    $this->attrs        = array();
    $this->unique_keys  = array();
    $this->sqls         = array();

    // Sets class name.
    $this->class_name = strtoupper($schema_name . '_' . $class_name);

    // Sets data types.
    $this->data_types = array(
      'NUMERIC' => array('serial', 'smallint', 'bigint', 'integer', 'double', 'real'),
      'DATE'    => array('timestamp', 'timestamp_without_time_zone'),
    );

    // Sets contraints.
    $this->constraints = array('unique', 'foreign');
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {}

  /**
   * Adds a unique key.
   *
   * @param array $info.
   */
  public function addUniqueKey($info) {
    $this->unique_keys[$info['name']] = $info['attrs'];
  }

  /**
   * Adds a SQL statement.
   *
   * @param string $type
   * @param string $sql
   */
  public function addSQL($type, $sql) {

    // Adds a SQL.
    if (in_array($type, $this->constraints)) {
      if (array_key_exists($type, $this->sqls)) {
        $this->sqls[$type][] = $sql;
      }
      else {
        $this->sqls[$type] = array($sql);
      }
    }
    else {
      $this->sqls[$type] = $sql;
    }
  }

  /**
   * Return the SQL statement.
   *
   * @param string $type
   *
   * @return string
   */
  public function getSQL($type) {
    $sqls = '';

    // Returns the SQL.
    if (in_array($type, $this->constraints)) {
      if (array_key_exists($type, $this->sqls)) {
        $arr = $this->sqls[$type];
        if ($arr) {
          $sqls = implode("\n", $arr);
        }
      }
    }
    else {
      if (array_key_exists($type, $this->sqls)) {
        $sqls = $this->sqls[$type];
      }
    }
    return $sqls;
  }

 /**
   * Returns the number of unique keys.
   *
   * @return integer.
   */
  public function getNumUniqueKeys() {
    return sizeof(array_keys($this->unique_keys));
  }

  /**
   * Checks if this table has primary key.
   *
   * @returns boolean TRUE|FALSE
   */
  public function hasPrimaryKey($info) {
    return !empty($this->primary_key);
  }

  /**
   * Checks if this table has unique keys.
   *
   * @returns boolean TRUE|FALSE
   */
  public function hasUniqueKey($info) {
    return !empty($this->unique_keys);
  }

  /**
   * Adds a table attribute.
   *
   * @param MCL_TABLE_ATTR $attr.
   *
   */
  public function addAttr(MCL_TABLE_ATTR $attr) {
    $this->attrs[$attr->getName()] = $attr;
  }

  /**
   * Print a schema for this table.
   *
   * @param MCL_TABLE_ATTR $attr.
   *
   */
  public function printSchema() {
    print "\n[" . $this->name . "]\n";
    foreach ($this->attrs as $attr_name => $attr_obj) {
      $attr_obj->printSchema();
    }
  }

  /**
   * Retrieves the attribute of the given name.
   *
   * @retrun MCL_TABLE_ATTR
   */
  public function getAttr($attr_name) {
    return $this->attrs[$attr_name];
  }

  /**
   * Updates the attribute of the given name.
   *
   * @param MCL_TABLE_ATTR $attr
   */
  public function setAttr($attr_name, $attr) {
    $this->attrs[$attr_name] = $attr;
  }

  /**
   * Retrieves the data type of the given attribute.
   *
   * @param MCL_TABLE_ATTR $attr
   */
  public function getDataType($attr_name) {
    $this->attrs[$attr_name]->getDataType();
  }

  /**
   * Updates the data type of the given attribute.
   *
   * @param MCL_TABLE_ATTR $attr
   */
  public function setDataType($attr_name, $type) {
    $this->attrs[$attr_name]->setDataType($type);
  }

  /**
   * Checks the data type of the given attribute.
   *
   * @retrun MCL_TABLE_ATTR
   */
  public function isDataTypeNumeric($attr_name) {
    $data_type = $this->getDataType($attr_name);
    return in_array($data_type, $this->data_types['NUMERIC']);
  }

  /**
   * Returns the data types of the provided type.
   *
   * @param $type
   *
   * @return array
   * Data types
   */
  public function getDataTypes($type) {
    return $this->data_types[$type];
  }

  // Defines getters and setters below.
  /**
   * Retrieves the name.
   *
   * @retrun string
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Updates the name.
   *
   * @param string $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the schema name.
   *
   * @retrun string
   */
  public function getSchemaName() {
    return $this->schema_name;
  }

  /**
   * Updates the schema name.
   *
   * @param string $schema_name
   */
  public function setSchemaName($schema_name) {
    $this->schema_name = $schema_name;
  }

  /**
   * Retrieves the class name.
   *
   * @retrun integer
   */
  public function getClassName() {
    return $this->class_name;
  }

  /**
   * Updates the class name.
   *
   * @param string $class_name
   */
  public function setClassName($class_name) {
    $this->class_name = $class_name;
  }

  /**
   * Retrieves the attributes.
   *
   * @retrun array
   */
  public function getAttrs() {
    return $this->attrs;
  }

  /**
   * Updates the attributes.
   *
   * @param array $attrs
   */
  public function setAttrs($attrs) {
    $this->attrs = $attrs;
  }

  /**
   * Retrieves the primary key.
   *
   * @retrun string
   */
  public function getPrimaryKey() {
    return $this->primary_key;
  }

  /**
   * Updates the primary key.
   *
   * @param array $primary_key
   */
  public function setPrimaryKey($primary_key) {
    $this->primary_key = $primary_key;
  }

  /**
   * Retrieves the unique keys.
   *
   * @retrun array
   */
  public function getUniqueKeys() {
    return $this->unique_keys;
  }

  /**
   * Updates the unique keys.
   *
   * @param array $unique_keys
   */
  public function setUniqueKeys($unique_keys) {
    $this->unique_keys = $unique_keys;
  }
}