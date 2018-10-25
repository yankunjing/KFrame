<?php

class Db_project extends MY_Model {
    
    private $_tableName = 'project';//表名

    function __construct()
    {
        parent::__construct();
    }
    //添加
    function add($infoAry){
        $res = $this->db->insert($this->_tableName, $infoAry);
        return $res;
    }
    //更新
    function update($id,$upAry){
        $id = intval($id);
        $res = $this->db->update($this->_tableName, $upAry,array('project_id'=>$id));
        return $res;
    }
    //列表
    function listAll(){
        $res = $this->db->from($this->_tableName)->get()->result_array();
        return $res;
    }
    //详情
    function detail($id){
        $id = intval($id);
        if(!$id) {
            return false;
        }
        $tmpAuth = $this->db->from($this->_tableName)->where(array('project_id' => $id))->get()->row_array();
        if(empty($tmpAuth)){
            return false;
        }
        return $tmpAuth;
    }
    //删除
    function delete($id){
        $id = intval($id);
        $res = $this->db->delete($this->_tableName,array('project_id'=>$id));
        return $res;
    }


}

/* end of file */